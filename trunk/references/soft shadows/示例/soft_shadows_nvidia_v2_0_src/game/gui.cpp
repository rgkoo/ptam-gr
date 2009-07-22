
// includes
#include "bleifrei/gfx/Font.hpp"
#include "bleifrei/gui/GUIManager.hpp"
#include "bleifrei/tex/TextureManager.hpp"

#include "Button.hpp"
#include "CheckBox.hpp"
#include "Frame.hpp"
#include "Label.hpp"
#include "ProgressIndicator.hpp"
#include "TextBox.hpp"

using namespace bleifrei::gfx;
using namespace bleifrei::gui;
using namespace bleifrei::math;
using namespace bleifrei::tex;
using namespace bleifrei;
using namespace std;

extern Font *font;
extern TextureManager *texture_manager;

extern string texture_file;

extern GUIManager *gui_manager;

extern bool mouse_invert;

Button *load_button;
TextBox *load_textbox;
CheckBox *invert_mouse_checkbox;

void load_map(string);

void button_callback(Button *button)
{
	if (button == load_button) {
		if (!load_textbox->getText().empty())
			load_map(load_textbox->getText());
	}
}

void init_gui(void)
{
	Frame *title_frame = new Frame(10, 30, platform::getScreenWidth() - 20, 35);
	Frame *frame = new Frame(10, 75, platform::getScreenWidth() - 20, 70);
	Frame *credits_frame = new Frame(10, 165, platform::getScreenWidth() - 20, 150);

	title_frame->add(new Label(Vector3(0.5f,1,0.6f), "DEMO: Soft Volumetric Shadows v2.0", font, 10, 10, 200, 20));

	load_textbox = new TextBox(font, 10, 10, platform::getScreenWidth() - 150, 20);
	load_button = new Button("Load map", font, platform::getScreenWidth() - 130, 10, 100, 20);
	load_button->setCallback(button_callback);

	invert_mouse_checkbox = new CheckBox("Invert mouse", font, 10, 40, 200, 20);
	invert_mouse_checkbox->setChecked(mouse_invert);

	frame->add(load_textbox);
	frame->add(load_button);
	frame->add(invert_mouse_checkbox);

	credits_frame->add(new Label(Vector3(1, 1, 1), "Homepage:", font, 10, 10, 200, 20));
	credits_frame->add(new Label(Vector3(0.5f, 1, 0.6f), "http://www.ce.chalmers.se/staff/tomasm/soft/", font, 20, 30, 200, 20));
	credits_frame->add(new Label(Vector3(0.5f, 1, 0.6f), "http://www.borgenstam.com/noname/", font, 20, 50, 200, 20));
	credits_frame->add(new Label(Vector3(1, 1, 1), "Code:", font, 10, 80, 200, 20));
	credits_frame->add(new Label(Vector3(0.5f, 1, 0.6f), "Jonas Svensson", font, 20, 100, 200, 20));
	credits_frame->add(new Label(Vector3(0.5f, 1, 0.6f), "Ulf Borgenstam", font, 20, 120, 200, 20));

	gui_manager = new GUIManager();
	gui_manager->add(title_frame);
	gui_manager->add(frame);
	gui_manager->add(credits_frame);
}

void update_gui(void)
{
	gui_manager->update();

	mouse_invert = invert_mouse_checkbox->isChecked();
}
