/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_lang_Lexer_hpp_
#define _include_bleifrei_lang_Lexer_hpp_


#include <istream>
#include <string>
#include <map>

namespace bleifrei {
	namespace lang {

		class Lexer {
		public:
			enum Token { Unknown, EndOfFile, Operator, Separator, Identifier, Keyword, Integer, Real, Character, String };

			class unexpected {
			public:
				unexpected(const std::string &expected, const std::string &found)
					: expected(expected), found(found) {}
				std::string expected;
				std::string found;
			};
			class newline_in_constant {};
			class illegal_hex_value {};
			class illegal_oct_value {};

			Lexer(std::istream &is,
				const std::string &opChar = "+-*/=",
				const std::string &sepChar = "{}[]();,")
				: line(1), col(0), token_line(-1), token_col(-1), is(is), opChar(opChar),
					sepChar(sepChar), unget(false), skipLinebreak(true),
					caseSensitive(true), cStrings(true)	{}

			bool getNextToken(void);
			void ungetToken(void)							{ unget = true;			}
			void flush(void)								{ unget = false;		}
			Token tokenType(void) const						{ return type;			}
			std::string tokenTypeString(void);

			int getKeywordId(void) const					{ return id;			}
			int getInteger(void) const						{ return integer;		}
			double getReal(void) const						{ return real;			}
			char getCharacter(void) const					{ return str[0];		}
			const std::string &getString(void) const		{ return str;			}
			const std::string &getOperator(void) const		{ return str;			}
			const std::string &getSeparator(void) const		{ return str;			}
			const std::string &getKeyword(void) const		{ return str;			}
			const std::string &getIdentifier(void) const	{ return str;			}

			int getLine(void) const							{ return token_line;	}
			int getColumn(void) const						{ return token_col;		}
			int getCurrentLine(void) const					{ return line;			}
			int getCurrentColumn(void) const				{ return col;			}

			void addKeyword(const std::string &keyword, int id)
			{
				this->keyword.insert(std::pair<std::string, int>(keyword, id));
			}

			bool isKeyword(const std::string &keyword, int &id) const;

			void setSkipLinebreak(bool skip = true)			{ skipLinebreak = skip;	}
			void setCaseSensitive(bool sens = true)			{ caseSensitive = sens;	}
			void setCStrings(bool cstr = true)				{ cStrings = cstr;		}
			void pushState(void);
			void popState(void);
			bool getChar(char &ch);
		protected:
			void ungetChar(void);
			bool getStringChar(char &ch);

			bool isopchar(char ch)
			{
				return (opChar.find(ch) != std::string::npos);
			}

			bool issepchar(char ch)
			{
				return (sepChar.find(ch) != std::string::npos);
			}
		private:
			Token type;
			int line, col;
			int token_line, token_col;
			bool unget;

			bool skipLinebreak;
			bool caseSensitive;
			bool cStrings;

			int id;
			int integer;
			double real;
			std::string str;

			std::string opChar;
			std::string sepChar;
			
			std::map<std::string, int> keyword;

			std::istream &is;

			// push state variables;
			std::istream::pos_type push_pos;
			std::string push_str;
			int push_id;
			int push_integer;
			double push_real;
			Token push_type;
			int push_line, push_col;
			int push_token_line, push_token_col;
		};

	}
}


#endif //_include_bleifrei_lang_Lexer_hpp_
