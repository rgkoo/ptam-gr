/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
#include <fstream>
#include <list>

#include "../platform/platform.hpp"

#include "console.hpp"

using namespace bleifrei::gfx;
using namespace std;

namespace bleifrei {
	namespace io {
		namespace console {

			class Line {
			public:
				Line(float r, float g, float b, const string &str) : r(r), g(g), b(b), text(str) {}
				float r, g, b;
				string text;
			};

			list<Line> lines;
			Font *font;

			int getLineCount(void)
			{
				return (int)lines.size();
			}

			void printf(float r, float g, float b, const char *str, ...)
			{
				char text[256];

				if (str == NULL) {
					print(r, g, b, "");
					return;
				}

				va_list	ap;
				va_start(ap, str);
				vsprintf(text, str, ap);
				va_end(ap);

				print(r, g, b, text);
			}

			void print(float r, float g, float b, const string &str)
			{
				lines.push_back(Line(r, g, b, str));
				platform::setConsoleColor(r, g, b);
				cout << str << endl;
			}

			void setFont(Font *f)
			{
				font = f;
			}

			void render(int first, int count)
			{
				int y = 0;
				list<Line>::iterator it;
				for (it = lines.begin(); it != lines.end() && count > 0; ++it) {
					if (--first <= 0) {
						count--;
						glColor3fv(&it->r);
						font->print(0, y, 1.0f, it->text);
						y += font->getHeight();
					}
				}
			}

			void saveLog(const string &filename)
			{
				ofstream ofs(filename.c_str());

				for (list<Line>::iterator it = lines.begin(); it != lines.end(); ++it) {
					ofs << it->text << endl;
				}
			}

		}
	}
}
