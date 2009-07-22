/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
#include <iostream>
#include "Parser.hpp"

using namespace std;

namespace bleifrei {
	namespace lang {

		bool Parser::test(Parser::Token &token)
		{
			if (!lex.getNextToken()) return false;
			if (lex.tokenType() == token.type) {
				switch (token.type) {
					case Lexer::Keyword :
						if (token.token == lex.getKeyword()) return true;
						break;
					case Lexer::Operator :
						if (token.token == lex.getKeyword()) return true;
						break;
					case Lexer::Separator :
						if (token.token == lex.getSeparator()) return true;
						break;
				}
			}
			lex.ungetToken();
			return false;
		}

		void Parser::parse(Token &token)
		{
			if (!lex.getNextToken()) throw expected(string("\'") + token.token + "\'", "end of stream", lex);
			if (lex.tokenType() == token.type) {
				switch (token.type) {
					case Lexer::Keyword :
						if (token.token == lex.getKeyword()) return;
						break;
					case Lexer::Operator :
						if (token.token == lex.getOperator()) return;
						break;
					case Lexer::Separator :
						if (token.token == lex.getSeparator()) return;
						break;
				}
			}
			throw expected(string("\'") + token.token + "\'", lex);
		}

		bool Parser::test(Identifier &identifier)
		{
			if (!lex.getNextToken()) return false;
			if (lex.tokenType() != Lexer::Identifier) {
				lex.ungetToken();
				return false;
			}
			identifier.str = lex.getIdentifier();
			return true;
		}

		void Parser::parse(Identifier &identifier)
		{
			if (!lex.getNextToken()) throw expected("identifier", "end of stream", lex);
			if (lex.tokenType() != Lexer::Identifier) throw expected("identifier", lex);
			identifier.str = lex.getIdentifier();
		}

		bool Parser::test(string &str)
		{
			if (!lex.getNextToken()) return false;
			if (lex.tokenType() != Lexer::String) {
				lex.ungetToken();
				return false;
			}
			str = lex.getString();
			return true;
		}

		void Parser::parse(string &str)
		{
			if (!lex.getNextToken()) throw expected("string", "end of stream", lex);
			if (lex.tokenType() != Lexer::String) throw expected("string", lex);
			str = lex.getString();
		}

		bool Parser::test(double &real)
		{
			if (!lex.getNextToken()) return false;
			if (lex.tokenType() != Lexer::Real) {
				lex.ungetToken();
				return false;
			}
			real = lex.getReal();
			return true;
		}

		void Parser::parse(double &real)
		{
			if (!lex.getNextToken()) throw expected("real", "end of stream", lex);
			if (lex.tokenType() != Lexer::Real) throw expected("real", lex);
			real = lex.getReal();
		}

		bool Parser::test(int &i)
		{
			if (!lex.getNextToken()) return false;
			if (lex.tokenType() != Lexer::Integer) {
				lex.ungetToken();
				return false;
			}
			i = lex.getInteger();
			return true;
		}

		void Parser::parse(int &i)
		{
			if (!lex.getNextToken()) throw expected("integer", "end of stream", lex);
			if (lex.tokenType() != Lexer::Integer) throw expected("integer", lex);
			i = lex.getInteger();
		}

		bool Parser::test(char &ch)
		{
			if (!lex.getNextToken()) return false;
			if (lex.tokenType() != Lexer::Character) {
				lex.ungetToken();
				return false;
			}
			ch = lex.getCharacter();
			return true;
		}

		void Parser::parse(char &ch)
		{
			if (!lex.getNextToken()) throw expected("character", "end of stream", lex);
			lex.ungetToken();
			if (!test(ch)) throw expected("character", lex);
		}

	}
}