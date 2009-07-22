/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_lang_Parser_hpp_
#define _include_bleifrei_lang_Parser_hpp_


// includes
#include <iostream>

#include "Lexer.hpp"

using namespace std;

namespace bleifrei {
	namespace lang {

		class expected {
		public:
			expected(const std::string &expect, Lexer &lex)
				: expect(expect), found(lex.tokenTypeString() + std::string(" \'") + lex.getString() + "\'"), line(lex.getLine()), col(lex.getColumn()) {}
			expected(const std::string expect, const std::string found, Lexer &lex)
				: expect(expect), found(found), line(lex.getLine()), col(lex.getColumn()) {}
			std::string expect, found;
			int line, col;
		};

		struct Identifier {
			std::string str;
		};

		template<class T>
		struct Matcher {
			Matcher(T &parser, bool valid = true) : parser(parser), valid(valid) {}
			operator bool(void)				{ return valid;		}
			T &parser;
			bool valid;
		};

		class Parser {
		public:
			struct Token {
				Token(int type, const std::string &token) : type(type), token(token) {}
				int type;
				std::string token;
			};
			Parser(Lexer &lex) : lex(lex) {}
			bool test(Token &);
			bool test(char &);
			bool test(int &);
			bool test(double &);
			bool test(std::string &);
			bool test(Identifier &);
			void parse(Token &);
			void parse(char &);
			void parse(int &);
			void parse(double &);
			void parse(Identifier &);
			void parse(std::string &);
		protected:
			Lexer &lex;
		};

		struct Sep : public Parser::Token {
			Sep(const std::string &token) : Parser::Token(Lexer::Separator, token) {}
		};

		struct Op : public Parser::Token {
			Op(const std::string &token) : Parser::Token(Lexer::Operator, token) {}
		};

		struct Keyword : public Parser::Token {
			Keyword(const std::string &token) : Parser::Token(Lexer::Keyword, token) {}
		};

	}
}


#endif //_include_bleifrei_lang_Parser_hpp_
