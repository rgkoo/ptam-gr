
// includes
#include <fstream>

#include "bleifrei/math/Matrix_op.hpp"

using namespace bleifrei::math;

#include "bleifrei/gfx/opengl.hpp"
#include "bleifrei/gfx/RenderTarget.hpp"
#include "bleifrei/gui/GUIManager.hpp"
#include "bleifrei/input/mouse.hpp"
#include "bleifrei/io/ConfigFile.hpp"
#include "bleifrei/io/console.hpp"
#include "bleifrei/lang/Parser.hpp"
#include "bleifrei/mesh/MeshManager.hpp"
#include "bleifrei/mesh/ModelManager.hpp"
#include "bleifrei/mesh/Model.hpp"
#include "bleifrei/platform/platform.hpp"
#include "bleifrei/tex/TextureManager.hpp"

#include "Button.hpp"
#include "Frame.hpp"
#include "Object.hpp"
#include "FPSWorld.hpp"

using namespace bleifrei::gfx;
using namespace bleifrei::gui;
using namespace bleifrei::input;
using namespace bleifrei::io;
using namespace bleifrei::mesh;
using namespace bleifrei::tex;
using namespace bleifrei;
using namespace std;

#define MAPNAME "default.map"

GUIManager *gui_manager;

Font *font;

World *world;

TextureManager *texture_manager;

void init_gui(void);
void update_gui(void);

float fps = 0.0f;

bool gui_mode = false;

void keytyped(char ch)
{
	if (gui_mode) gui_manager->keyTyped(ch);
}

void keydown(int key)
{
	if (gui_mode) gui_manager->keyDown(key);
}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, width/(float)height, 1, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void load_map(string filename)
{
	try {
		ConfigFile config_file(ifstream(filename.c_str()));
		delete world;
		world = new FPSWorld(config_file);
	}
	catch (bleifrei::lang::expected &e) {
		int col = e.col;
		int line = e.line;
		std::string s1 = e.expect;
		std::string s2 = e.found;
		console::printf(1, 0, 0, "Error(ConfigFile): Expected %s, found %s on line %i in column %i", s1.c_str(), s2.c_str(), line, col);
	}
	gui_mode = false;
}

void render(void)
{
	world->render();


	RenderTarget::enableOrtho(platform::getScreenWidth(), platform::getScreenHeight());
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1.5f);
	glLoadIdentity();

	glColor3f(1,1,1);
	font->printf( 5, 5, 1.0f, "FPS: %.2f", fps);

	if (gui_mode) {
		Texture2::disable();
		gui_manager->render();
		glColor4f(0,1,0,0.5f);
		int mousex, mousey;
		mouse::getPosition(mousex, mousey);
		glBegin(GL_TRIANGLES);
			glVertex2i(mousex, mousey);
			glVertex2i(mousex, mousey + 20);
			glVertex2i(mousex + 5, mousey + 15);
			glVertex2i(mousex, mousey);
			glVertex2i(mousex + 5, mousey + 15);
			glVertex2i(mousex + 15, mousey + 15);
		glEnd();
		glColor4f(0,1,0,1.0f);
		glBegin(GL_LINE_LOOP);
			glVertex2i(mousex, mousey);
			glVertex2i(mousex, mousey + 20);
			glVertex2i(mousex + 5, mousey + 15);
			glVertex2i(mousex + 15, mousey + 15);
		glEnd();
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	RenderTarget::disableOrtho();
}

inline float randf(void)
{
	return (rand() % 255) / 256.0f;
}

void update(void)
{
	static float time = 0.0f;
	static int frame_count = 0;

	if (time >= 1.0f) {
		float time_inv = 1 / time;
		fps = frame_count * time_inv;
		time = 0.0f;
		frame_count = 0;
	}
	time += platform::getFrameStep();
	frame_count++;

	if (GetAsyncKeyState(VK_ESCAPE)) platform::quit();

	static bool toggleF1 = false;
	if (GetAsyncKeyState(VK_F1) & 0x8000) {
		if (!toggleF1) {
			gui_mode = !gui_mode;
			if (gui_mode)
				mouse::setMode(mouse::Absolute);
			else
				mouse::setMode(mouse::Relative);
			toggleF1 = true;
		}
	}
	else toggleF1 = false;

	if (!gui_mode) {
		world->update();
	}
	else {
		update_gui();
	}
}

bool initApplication(void)
{
	platform::createConsole();
	platform::createWindow("game", 640, 480, 32, true);	
	platform::setRenderFunc(render);
	platform::setUpdateFunc(update);
	platform::setResizeFunc(resize);
	platform::setKeyboardFunc(keytyped);
	platform::setSpecialKeyFunc(keydown);
	mouse::setMode(mouse::Relative);

	init_opengl();
	init_GL_ARB_vertex_program();
	init_GL_ARB_fragment_program();

	init_WGL_ARB_pixel_format();
	init_WGL_ARB_make_current_read();
	init_WGL_ARB_pbuffer();
	init_WGL_ARB_render_texture();

	init_GL_ATI_separate_stencil();

	init_WGL_ATI_pixel_format_float();
	
	init_GL_NV_texture_rectangle();
	init_GL_NV_float_buffer();

	init_WGL_NV_render_texture_rectangle();
	init_WGL_NV_float_buffer();

	init_GL_EXT_texture_filter_anisotropic();
	init_GL_EXT_stencil_two_side();

	texture_manager = new TextureManager("../textures/");
	try {
		ConfigFile config_file(ifstream("font.fnt"));
		font = new Font(config_file, *texture_manager);
	}
	catch (bleifrei::lang::expected &e) {
		int col = e.col;
		int line = e.line;
		std::string s1 = e.expect;
		std::string s2 = e.found;
		console::printf(1, 0, 0, "Error(font.fnt): Expected %s, found %s on line %i in column %i", s1.c_str(), s2.c_str(), line, col);
	}

	// LOAD WORLD OBJECT HERE!
	try {
		ConfigFile config_file(ifstream(MAPNAME));
		world = new FPSWorld(config_file);
	}
	catch (bleifrei::lang::expected &e) {
		int col = e.col;
		int line = e.line;
		std::string s1 = e.expect;
		std::string s2 = e.found;
		console::printf(1, 0, 0, "Error(ConfigFile): Expected %s, found %s on line %i in column %i", s1.c_str(), s2.c_str(), line, col);
	}

	init_gui();

	Texture2::enable();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

void destroyApplication(void)
{
	delete gui_manager;

	// DESTROY WORLD OBJECT HERE
	delete world;

	delete font;
	delete texture_manager;

	platform::destroyWindow();
	platform::destroyConsole();
}
