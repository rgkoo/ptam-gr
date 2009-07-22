
// includes
#include "../lang/Lexer.hpp"
#include "../lang/Parser.hpp"
#include "../math/Matrix_op.hpp"

#include "ConfigFile.hpp"

using namespace bleifrei::lang;
using namespace bleifrei::math;
using namespace std;

namespace bleifrei {
	namespace io {

		struct IntValue : public ConfigFile::Value {
			IntValue(int value) : value(value) {}
			virtual void get(int &value);
			virtual void get(double &value);
			virtual void print(ostream &os);
			int value;
		};

		struct RealValue : public ConfigFile::Value {
			RealValue(double value) : value(value) {}
			virtual void get(double &value);
			virtual void print(ostream &os);
			double value;
		};

		struct StringValue : public ConfigFile::Value {
			StringValue(const string &value) : value(value) {}
			virtual void get(string &value);
			virtual void print(ostream &os);
			string value;
		};

		struct CharacterValue : public ConfigFile::Value {
			CharacterValue(char value) : value(value) {}
			virtual void get(char &value);
			virtual void print(ostream &os);
			char value;
		};

		struct BoolValue : public ConfigFile::Value {
			BoolValue(bool value) : value(value) {}
			virtual void get(bool &value);
			virtual void print(ostream &os);
			bool value;
		};

		struct Vector3Value : public ConfigFile::Value {
			Vector3Value(const Vector3 &value) : value(value) {}
			virtual void get(Vector3 &value);
			virtual void get(Vector4 &value);
			virtual void print(ostream &os);
			Vector3 value;
		};

		struct Vector4Value : public ConfigFile::Value {
			Vector4Value(const Vector4 &value) : value(value) {}
			virtual void get(Vector3 &value);
			virtual void get(Vector4 &value);
			virtual void print(ostream &os);
			Vector4 value;
		};

		void ConfigFile::Value::get(int &value)		{}
		void ConfigFile::Value::get(double &value)	{}
		void ConfigFile::Value::get(char &value)	{}
		void ConfigFile::Value::get(bool &value)	{}
		void ConfigFile::Value::get(string &value)	{}
		void ConfigFile::Value::get(Vector3 &value)	{}
		void ConfigFile::Value::get(Vector4 &value)	{}

		void IntValue::get(int &value)				{ value = this->value;			}
		void IntValue::get(double &value)			{ value = this->value;			}
		void RealValue::get(double &value)			{ value = this->value;			}
		void BoolValue::get(bool &value)			{ value = this->value;			}
		void CharacterValue::get(char &value)		{ value = this->value;			}
		void StringValue::get(string &value)		{ value = this->value;			}
		void Vector3Value::get(Vector3 &value)		{ value = this->value;			}
		void Vector3Value::get(Vector4 &value)		{ assign(value, this->value);	}
		void Vector4Value::get(Vector3 &value)		{ assign(value, this->value);	}
		void Vector4Value::get(Vector4 &value)		{ value = this->value;			}

		void IntValue::print(ostream &os)			{ os << value;					}
		void RealValue::print(ostream &os)			{ os << value;					}
		void BoolValue::print(ostream &os)
		{
			os << (value ? "true" : "false");
		}
		void CharacterValue::print(ostream &os)
		{
			os << '\'' << value << '\'';
		}
		void StringValue::print(ostream &os)
		{
			os << '\"' << value << '\"';
		}
		void Vector3Value::print(ostream &os)
		{
			os << '{' << value[0] << ", " << value[1] << ", " << value[2] << " }";
		}
		void Vector4Value::print(ostream &os)
		{
			os << "{ " << value[0] << ", " << value[1] << ", " << value[2] << ", " << value[3] << " }";
		}

		class ConfigLexer : public Lexer {
		public:
			ConfigLexer(istream &is) : Lexer(is, "=-", "{},[]\n")
			{
				setCStrings(false);
			}
		};

		class ConfigParser : public Parser {
		public:
			struct Matcher {
				Matcher(ConfigParser &parser, bool valid = true) : parser(parser), valid(valid) {}
				operator bool(void)			{ return valid;		}
				ConfigParser &parser;
				bool valid;
			};

			ConfigParser(ConfigLexer &lex) : Parser(lex)
			{
				lex.addKeyword("false", 0);
				lex.addKeyword("true", 0);
			}
			bool test(ConfigFile::Value *&);
			template<class T> bool test(T &token)	{ return Parser::test(token);	}
			void parse(ConfigFile::Value *&);
			template<class T> void parse(T &token)	{ Parser::parse(token);			}
			double read_real(void);
		};

		template<class T>
			inline ConfigParser::Matcher operator <<(ConfigParser &parser, T &type)
		{
			if (parser.test(type))
				return ConfigParser::Matcher(parser);
			else
				return ConfigParser::Matcher(parser, false);
		}
		
		template<class T>
			inline ConfigParser::Matcher &operator <<(ConfigParser::Matcher &matcher, T &type)
		{
			if (matcher.valid) matcher.parser.parse(type);
			return matcher;
		}

		double ConfigParser::read_real(void)
		{
			int i;
			double f;

			if (test(Op("-"))) {
				if (test(i))
					f = (double)i;
				else
					parse(f);
				f = -f;
			}
			else {
				if (test(i))
					f = (double)i;
				else
					parse(f);
			}
			return f;
		}

		bool ConfigParser::test(ConfigFile::Value *&value)
		{
			std::string str;
			double real, x, y, z, w;
			int i;
			char ch;
			if (test(ch)) {
				value = new CharacterValue(ch);
			}
			else if (test(Op("-"))) {
				if (test(real)) {
					value = new RealValue(-real);
				}
				else {
					parse(i);
					value = new IntValue(-i);
				}
			}
			else if (test(Sep("{"))) {
				x = read_real();
				parse(Sep(","));
				y = read_real();
				parse(Sep(","));
				z = read_real();

				if (test(Sep(","))) {
					w = read_real();
					parse(Sep("}"));
					value = new Vector4Value(Vector4((Vector3::value_type)x, (Vector3::value_type)y, (Vector3::value_type)z, (Vector3::value_type)w));
				}
				else {
					parse(Sep("}"));
					value = new Vector3Value(Vector3((Vector3::value_type)x, (Vector3::value_type)y, (Vector3::value_type)z));
				}
			}
			else if (test(real)) {
				value = new RealValue(real);
			}
			else if (test(i)) {
				value = new IntValue(i);
			}
			else if (test(Keyword("true"))) {
				value = new BoolValue(true);
			}
			else if (test(Keyword("false"))) {
				value = new BoolValue(false);
			}
			else if (test(str)) {
				value = new StringValue(str);
			}
			else {
				value = 0;
				return false;
			}
			return true;
		}

		void ConfigParser::parse(ConfigFile::Value *&value)
		{
			if (!lex.getNextToken()) throw expected("value", "end of stream", lex);
			lex.ungetToken();
			if (!test(value)) throw expected("value", lex);
		}

		ConfigFile::ConfigFile(istream &is)
			: currentSection(0)
		{
			read(is);
		}

		ConfigFile::~ConfigFile(void)
		{
			clear();
		}

		void ConfigFile::clear(void)
		{
			map<string, map<string, Value *> >::iterator section_it;

			for (section_it = sections.begin(); section_it != sections.end(); ++section_it) {
				map<string, Value *>::iterator it;
				for (it = section_it->second.begin(); it != section_it->second.end(); ++it) {
					delete it->second;
				}
			}
			sections.clear();
		}

		void ConfigFile::read(istream &is)
		{
			ConfigLexer clex(is);
			ConfigParser parser(clex); 

			Identifier section;
			while (parser << Sep("[") << section << Sep("]")) {
				createSection(section.str);
				Identifier key;
				Value *value;
				while (parser << key << Op("=") << value) {
					assign(key.str, value);
				}
			}
			if (clex.tokenType() != Lexer::EndOfFile) {
				throw expected("section", clex);
			}
		}

		void ConfigFile::write(ostream &os)
		{
			map<string, map<string, Value *> >::iterator section_it;

			for (section_it = sections.begin(); section_it != sections.end(); ++section_it) {
				os << '[' << section_it->first << ']' << endl;

				map<string, Value *>::iterator it;
				for (it = section_it->second.begin(); it != section_it->second.end(); ++it) {
					os << it->first << " = ";
					it->second->print(os);
					os << endl;
				}
				os << endl;
			}
		}

		void ConfigFile::createSection(const string &section)
		{
			currentSection = &sections[section];
		}

		void ConfigFile::createSection(const string &section, int index)
		{
			char buffer[20];
			createSection(section + itoa(index, buffer, 10));
		}

		void ConfigFile::selectSection(const string &section)
		{
			map<string, map<string, Value*> >::iterator it = sections.find(section);

			if (it == sections.end()) {
				currentSection = 0;
				return;
			}

			currentSection = &it->second;
		}

		void ConfigFile::selectSection(const string &section, int index)
		{
			char buffer[20];
			selectSection(section + itoa(index, buffer, 10));
		}

		void ConfigFile::set(const std::string &key, int value)
		{
			assign(key, new IntValue(value));
		}
		void ConfigFile::set(const std::string &key, bool value)
		{
			assign(key, new BoolValue(value));
		}
		void ConfigFile::set(const std::string &key, char value)
		{
			assign(key, new CharacterValue(value));
		}
		void ConfigFile::set(const std::string &key, double value)
		{
			assign(key, new RealValue(value));
		}
		void ConfigFile::set(const std::string &key, const std::string &value)
		{
			assign(key, new StringValue(value));
		}
		void ConfigFile::set(const std::string &key, const Vector3 &value)
		{
			assign(key, new Vector3Value(value));
		}
		void ConfigFile::set(const std::string &key, const Vector4 &value)
		{
			assign(key, new Vector4Value(value));
		}

		void ConfigFile::assign(const string &key, Value *value)
		{
			if (currentSection == 0) {
				delete value;
				return;
			}

			map<string, Value *>::iterator it = currentSection->find(key);

			if (it == currentSection->end()) {
				currentSection->insert(make_pair(key, value));
			}
			else {
				delete it->second;
				it->second = value;
			}
		}

	}
}