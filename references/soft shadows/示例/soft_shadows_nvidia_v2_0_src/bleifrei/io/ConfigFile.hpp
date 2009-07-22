
#ifndef _include_bleifrei_io_ConfigFile_hpp_
#define _include_bleifrei_io_ConfigFile_hpp_

// includes
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "../math/Matrix.hpp"

namespace bleifrei {
	namespace io {


		class ConfigFile {
		public:
			struct Value {
				virtual void get(int &value);
				virtual void get(double &value);
				virtual void get(char &value);
				virtual void get(bool &value);
				virtual void get(std::string &value);
				virtual void get(bleifrei::math::Vector3 &value);
				virtual void get(bleifrei::math::Vector4 &value);
				virtual void print(std::ostream &os) = 0;
			};

			ConfigFile(void) : currentSection(0) {}
			ConfigFile(std::istream &is);
			~ConfigFile(void);

			void read(std::istream &is);
			void write(std::ostream &os);
			void createSection(const std::string &section);
			void createSection(const std::string &section, int index);
			void selectSection(const std::string &section);
			void selectSection(const std::string &section, int index);
			template<class T> void set(const std::string &, int index, T);
			template<class T> void get(const std::string &, int index, T &);
			template<class T> void get(const std::string &, T &);
			void get(const std::string &, float &);
			void set(const std::string &key, int value);
			void set(const std::string &key, bool value);
			void set(const std::string &key, char value);
			void set(const std::string &key, double value);
			void set(const std::string &key, const std::string &value);
			void set(const std::string &key, const bleifrei::math::Vector3 &value);
			void set(const std::string &key, const bleifrei::math::Vector4 &value);

			void clear(void);

		private:
			void assign(const std::string &key, Value *);

			std::map<std::string, std::map<std::string, Value *> > sections;
			std::map<std::string, Value *> *currentSection;
		};

		template<class T>
			inline void ConfigFile::set(const std::string &key, int index, T value)
		{
			char buffer[20];
			set(key + itoa(index, buffer, 10), value);
		}

		template<class T>
			inline void ConfigFile::get(const std::string &key, int index, T &value)
		{
			char buffer[20];
			get(key + itoa(index, buffer, 10), value);
		}

		template<class T>
			inline void ConfigFile::get(const std::string &key, T &value)
		{
			if (currentSection == 0) return;

			std::map<std::string, Value *>::iterator it = currentSection->find(key);

			if (it == currentSection->end()) return;

			it->second->get(value);
		}

		inline void ConfigFile::get(const std::string &key, float &value)
		{
			double temp;

			get(key, temp);
			value = (float)temp;
		}

	}
}


#endif _include_bleifrei_io_ConfigFile_hpp_
