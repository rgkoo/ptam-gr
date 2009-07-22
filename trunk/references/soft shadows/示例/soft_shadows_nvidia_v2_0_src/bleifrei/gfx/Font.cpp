/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#include <string>
#include <cstdio>
#include <cstring>
#include <cstdarg>

#include "../tex/TextureManager.hpp"

#include "Font.hpp"

using namespace bleifrei::tex;
using namespace bleifrei::io;
using namespace std;

namespace bleifrei {
	namespace gfx {

		Font::Font(ConfigFile &config_file, TextureManager &tm) {
			string filename;
			int imagewidth = 256, imageheight = 256;
			height = 16;
			
			config_file.selectSection("Font");
			config_file.get("name", name);

			config_file.selectSection("Image");
			config_file.get("filename", filename);
			config_file.get("width", imagewidth);
			config_file.get("height", imageheight);
			
			if (filename == "") return;

			this->texture = tm.load(filename);

			config_file.selectSection("Characters");
			config_file.get("height", height);
			
			charBase = glGenLists(256);								// Create storage for characters
			
			for (int i = 0; i < 256; i++) {							// For each character
				int character_width = 0;										// Save width of character
				config_file.get("width", i, character_width);
				width[i] = character_width;
				config_file.get("character_width", i, character_width);

				float cx = float(i % 16) / 16.0f;
				float cy = float(i / 16) / 16.0f;

				float width = 0.0625f * (float)character_width / (imagewidth / 16.0f);
				float height = 0.0625f * (float)this->height / (imageheight / 16.0f);

				glNewList(charBase + i, GL_COMPILE);				// Select character display list
					glBegin(GL_QUADS);								// Create new Quad
						glTexCoord2f(cx, 1.0f - (cy + height));		// Bottom left corner
						glVertex2d(0, this->height);
						
						glTexCoord2f(cx + width , 1.0f - (cy + height));	// Bottom right corner
						glVertex2i(character_width, this->height);
						
						glTexCoord2f(cx + width, 1.0f - cy);		// Top right corner
						glVertex2i(character_width, 0);
						
						glTexCoord2f(cx, 1.0f - cy);				// Top left corner
						glVertex2i(0, 0);
					glEnd();										// Character quad finished
					
					glTranslated(this->width[i], 0, 0);				// Move "cursor"
				glEndList();										// Character finished
			}
		}

		/** Destroy font object.
		 */
		Font::~Font() {
			glDeleteLists(charBase, 256);							// Delete display lists
		}

		/** Print a formated string at position (x,y).
		 * @param "int x" Horisontal position
		 * @param "int y" Vertical position
		 * @param "float scale" Font is scaled by this number (TODO: Remove this?);
		 * @param "char *str" Character array to print (same format as for stdio-printf)
		 * @param ... Extra parameters to print
		 * @returns void
		 */
		void Font::printf(GLint x, GLint y, GLfloat scale, const char *str, ...) {

			// If there is no text just return
			if (str == NULL) {
				return;
			}

			// Output storage
			char text[256];
			
			// Argument list pointer
			va_list	ap;

			// Parse string and save in text
			va_start(ap, str);
			vsprintf(text, str, ap);
			va_end(ap);

			print(x, y, scale, (int)strlen(text), text);
		}

		/** Print a string at position (x,y).
		 * @param "int x" Horisontal position
		 * @param "int y" Vertical position
		 * @param "float scale" Font is scaled by this number (TODO: Remove this?);
		 * @param "int numchars" Number of characters to print
		 * @param "char *str" Character array to print
		 * @returns void
		 */
		void Font::print(GLint x, GLint y, GLfloat scale, int numchars, const char *str) {
			glPushMatrix();
			glEnable(GL_ALPHA_TEST);

			// Position text
			glTranslatef((float)x, (float)y, 0.0f);

			// If there is no text just return
			if (str == NULL) {
				return;
			}

			// Set texture
			bindTexture();

			// Load font
			glListBase(charBase);

			// Should we scale?
			if (scale != 1.0f) {
				glScalef(scale, scale, 1.0f);
			}

			// Draw string on screen
			glCallLists(numchars, GL_UNSIGNED_BYTE, str);

			glDisable(GL_ALPHA_TEST);
			glPopMatrix();
		}

		/** Get the maximum number of chars that will fit within the specified width.
		 * @param "char *str" Character array to read from
		 * @param "int numchars" Number of characters in array
		 * @param "int maxWidth" Maximum width
		 * @returns An \c integer specifying the number of characters from the string that
		 * will fit within \c maxWidth pixels.
		 */
		int Font::getMaxChars(const char *str, int numchars, int maxWidth) {

			// Set initial width to zero
			int width = 0;

			// Accumulate character widths
			for (int i = 0; i < numchars && str[i]; i++) {
				width += this->width[str[i]];

				// Make sure we do not go above maximum
				if (width > maxWidth) break;
			}

			return i;
		}

		/** Get the pixel width of a sequence of characters.
		 * @param "char *str" Character array containing characters
		 * @param "int numchars" Number of characters in array
		 * @returns An integer specifying the pixel width of the string.
		 */
		int Font::getWidth(const char *str, int numchars) {
			
			// Set initial width to zero
			int width = 0;

			// Accumulate character widths
			for (int i = 0; i < numchars && str[i]; i++) {
				width += this->width[str[i]];
			}

			return width;
		}

	}
}