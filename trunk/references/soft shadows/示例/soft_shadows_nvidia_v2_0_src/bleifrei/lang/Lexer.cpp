/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#include <cctype>

#include "Lexer.hpp"

using namespace std;

namespace bleifrei {
	namespace lang {

		string Lexer::tokenTypeString(void)
		{
			switch (type) {
				case Operator :
					return "operator";
				case Separator :
					return "separator";
				case Identifier :
					return "identifier";
				case Keyword :
					return "keyword";
				case Integer :
					return "integer";
				case Real :
					return "real";
				case Character :
					return "character";
				case String :
					return "string";
				case EndOfFile :
					return "end of file";
				default:
					return "unknown";
			}
		}

		void Lexer::pushState(void)
		{
			push_pos = is.tellg();
			push_str = str;
			push_id = id;
			push_integer = integer;
			push_real = real;
			push_type = type;
			push_line = line;
			push_col = col;
			push_token_line = token_line;
			push_token_col = token_col;
		}

		void Lexer::popState(void)
		{
			is.seekg(push_pos);
			str = push_str;
			id = push_id;
			integer = push_integer;
			real = push_real;
			type = push_type;
			line = push_line;
			col = push_col;
			token_line = push_token_line;
			token_col = push_token_col;
		}

		bool Lexer::getChar(char &ch) {
			is.get(ch);
			if (!is) return false;

			if (ch == '\n') {
				col = 0;
				++line;
			}
			else {
				++col;
			}

			return true;
		}

		void Lexer::ungetChar(void) {
			is.unget();
			--col;							// WARNING: Unget '\n' does not work
		}

		bool Lexer::isKeyword(const string &keyword, int &id) const
		{
			map<string, int>::const_iterator it = this->keyword.find(keyword);

			if (it == this->keyword.end()) return false;

			id = it->second;

			return true;
		}

		bool Lexer::getStringChar(char &ch)
		{
			if (!cStrings) return true;
			if (!getChar(ch)) return false;
			if (ch == 'x') {
				if (!getChar(ch)) return true;
				
				int value = 0;
				for (int i = 0; i < 2; ++i) {
					if (isdigit(ch)) {
						value *= 16;
						value += ch - '0';
					}
					else if (toupper(ch) >= 'A' && toupper(ch) <= 'F') {
						value *= 16;
						value += toupper(ch) - 'A' + 10;
					}
					else if (i == 0) {
						throw illegal_hex_value();
					}
					else {
						break;
					}
					if (!getChar(ch)) return false;
				}
				ungetChar();
				ch = value;
			}
			else if (ch >= '0' && ch <= '7') {
				int value = ch - '0';
				if (!getChar(ch)) return false;

				for (int i = 0; i < 2; ++i) {
					if (ch >= '0' && ch <= '7') {
						value *= 8;
						value += ch - '0';
					}
					else {
						break;
					}
					if (!getChar(ch)) return false;
				}
				ungetChar();
				if (value > 0xff) throw illegal_oct_value();
				ch = value;
			}
			else {
				switch (ch) {
					case 'a' :
						ch = '\a';
						break;
					case 'b' :
						ch = '\b';
						break;
					case 'f' :
						ch = '\f';
						break;
					case 'n' :
						ch = '\n';
						break;
					case 'r' :
						ch = '\r';
						break;
					case 't' :
						ch = '\t';
						break;
					case 'v' :
						ch = '\v';
						break;
					case '\\' :
						ch = '\\';
						break;
					case '\?' :
						ch = '\?';
						break;
					case '\'' :
						ch = '\'';
						break;
					case '\"' :
						ch = '\"';
						break;
				}
			}
			return true;
		}

		bool Lexer::getNextToken(void)
		{
			if (unget) {
				unget = false;
				return true;
			}
			char ch;
			
			type = EndOfFile;

			do {
				do {										// Remove all whitecharacters
					if (!getChar(ch)) return false;
					if (!skipLinebreak && ch == '\n') {
						type = Separator;
						str = '\n';
						return true;
					}
				} while (isspace(ch));
				

				while (ch == '/') {							// Remove comments
					int tcol = col, tline = line;
					if (getChar(ch)) {
/*						if (ch == '*') {					// C-style
							do {
								do {
									if (!getChar(ch)) throw unexpected("* /", "_End of stream_");
								} while (ch != '*');
								if (!getChar(ch)) throw unexpected("* /", "_End of stream_");
							} while (ch != '/');
							if (!getChar(ch)) throw unexpected("* /", "_End of stream_");
						}
						else*/
						if (ch == '/') {				// C++-style
							do {
								if (!getChar(ch)) return false;
							} while (ch != '\n');
							if (!getChar(ch)) return false;
						}
						else {								// Not a comment
							ungetChar();
							ch = '/';
							col = tcol;
							line = tline;
						}
					}
				}
			} while (isspace(ch));

			token_line = line;
			token_col = col;
			str = "";

			// Use character to determine the token type
			if (ch == '\'') {								// Character
				type = Character;
				if (!getChar(ch)) throw unexpected("matching \'", "_End of stream_");

				if (ch == '\\') {
					if (!getStringChar(ch)) throw unexpected("character constant", "_End of stream_");
				}
				else if (ch == '\'') {
					throw unexpected("character constant", "");
				}
				else if (ch == '\n') {
					throw newline_in_constant();
				}
				str += ch;
				if (!getChar(ch)) throw unexpected("matching \'", "_End_of_stream_");
				if (ch != '\'') throw unexpected("matching \'", string("") + ch);

				return true;
			}
			else if (ch == '\"') {							// String
				if (!getChar(ch)) throw unexpected("matching \"", "_End of stream_");

				while (ch != '\"') {
					if (ch == '\n') {
						throw newline_in_constant();
					}
					else if (ch == '\\') {
						if (!getStringChar(ch)) throw unexpected("matching \"", "_End of stream_");
					}
					str += ch;
					if (!getChar(ch)) throw unexpected("matching \"", "_End of stream_");
				}

				type = String;
				return true;
			} else if (isalpha(ch) || ch == '_') {		// Identifier or keyword

				// Read all alphanumeric characters
				string lcase;
				do {
					lcase += tolower(ch);
					str += ch;
					if (!getChar(ch)) break;
				} while (isalnum(ch) || ch == '_');
				
				if (!isspace(ch)) ungetChar();

				if (isKeyword(str, id)) {
					type = Keyword;
					if (!caseSensitive) str = lcase;
				}
				else {
					type = Identifier;
					if (!caseSensitive) str = lcase;
				}

				return true;
			}
			else if (isdigit(ch)) {						// Integer or real
				type = Integer;

				do {									// Read integer digits
					str += ch;
					if (!getChar(ch)) {
						ch = ' ';
						break;
					}
				} while (isdigit(ch));

				if (isalpha(ch) || ch == '_') {			// Illegal integer
					type = Unknown;

					do {
						str += ch;
					} while (getChar(ch) && (isalnum(ch) || ch == '_'));
					return true;
				}
				else if (ch == '.') {					// Read floating point value
					do {
						str += ch;
					} while (getChar(ch) && isdigit(ch));

					if (ch == 'e' || ch == 'E') {		// Value has exponent
						str += ch;
						if (!getChar(ch)) {
							type = Unknown;
							return true;
						}
						if (ch == '-' || ch == '+') {	// Value has explicit sign
							str += ch;
							if (!getChar(ch)) {
								type = Unknown;
								return true;
							}
						}
						if (!isdigit(ch)) {				// Must be digits in exponent
							str += ch;
							type = Unknown;
							return true;
						}
						do {							// Read exponent
							str += ch;
						} while (getChar(ch) && isdigit(ch));
					}

					if (isalpha(ch)) {					// Illegal Real
						do {
							str += ch;
						} while (getChar(ch) && isalnum(ch));

						type = Unknown;
						return true;
					}

					if (!isspace(ch)) ungetChar();
					type = Real;
					real = atof(str.c_str());
					return true;
				}
				else {
					if (!isspace(ch)) ungetChar();
					type = Integer;

					integer = atoi(str.c_str());
					return true;
				}
			}
			else if (isopchar(ch)) {						// Operator
				type = Operator;

				do {
					str += ch;
				} while (getChar(ch) && isopchar(ch));
				if (!isspace(ch)) ungetChar();
				return true;
			}
			else if (issepchar(ch)) {						// Separator
				type = Separator;

				str += ch;
				return true;
			}
			else {											// Unknown token
				type = Unknown;

				str += ch;

				return true;
			}

			return false;
		}

	}
}