#include <iostream>
// includes
#include "GUIComponent.hpp"

namespace bleifrei {
	namespace gui {

		GUIComponent::GUIComponent(void)
			: x(0), y(0), children(), visible(true), enabled(true), mouseover(false), clicked(false), wantfocus(false), hasfocus(false)
		{
		}

		GUIComponent::~GUIComponent(void) {
		}

		void GUIComponent::cleanup(void)
		{
			while (!children.empty()) {
				children.back()->cleanup();
				delete children.back();
				children.pop_back();
			}
		}

		/** Signals the object to repaint itself.
		 * @param "int absx" Absolute x position of parent
		 * @param "int absy" Absolute y position of parent
		 */
		void GUIComponent::paint(int absx, int absy)
		{
			if (!visible) return;
			
			absx += x;														// Calcultate absolute coordinates of object
			absy += y;

			this->onPaintBackground(absx, absy);							// Draw background layer
			for (CONTAINER::iterator it = children.begin(); it != children.end(); ++it) {	// Draw children
				(*it)->paint(absx, absy);
			}
			this->onPaintOverlay(absx, absy);								// Draw top layer
		}

		bool GUIComponent::mouseDown(int button, int x, int y)
		{
			if (!enabled) return false;										// If not enabled, do not intercept

			x -= this->x;													// Calculate local coordinates
			y -= this->y;

			// Send event to children
			for (CONTAINER::reverse_iterator it = children.rbegin(); it != children.rend(); ++it) {
				if ((*it)->mouseDown(button, x, y)) {						// Send event and check status
					giveFocus(it);
					return true;
				}
			}

			if ((x >= 0) && (x < width) && (y >= 0) && (y < height)) {		// Did event occur inside component?
				if (button == LeftButton) {									// Was it the left button?
					clicked = true;
					this->onMouseDown(x, y);
					return true;
				}
				else if (button == RightButton) {							// ...or perhaps the right one?
					this->onRightMouseDown(x, y);
					return true;
				}
			}
			return false;
		}

		void GUIComponent::mouseUp(int button, int x, int y)
		{
			if (!enabled) return;											// If not enabled, do not intercept

			x -= this->x;													// Calculate local coordinates
			y -= this->y;

			// Send event to children
			for (CONTAINER::reverse_iterator it = children.rbegin(); it != children.rend(); ++it) {
				(*it)->mouseUp(button, x, y);								// Send event
			}

			if ((x >= 0) && (x < width) && (y >= 0) && (y < height)) {		// Did event occur inside component?
				if (button == LeftButton) {									// Was it the left button?

					if (clicked) {
						this->onClick(x, y);
					}
					clicked = false;
					this->onMouseUp(x, y);
				}
				else if (button == RightButton) {							// ...or perhaps the right one?
					this->onRightMouseUp(x, y);
				}
			}
			else {															// Mouse event occured outside limits
				if (clicked && button == LeftButton) {
					clicked = false;
					this->onMouseUp(x,y);
				}
				if (button == RightButton) {
					this->onRightMouseUp(x,y);
				}
			}
		}

		/* motion
			Capture mouse motion

			x, y - mouse coordinates
		*/
		void GUIComponent::motion(int x, int y)
		{
			if (!enabled) return;											// If not enabled do not intercept

			x -= this->x;													// Calculate local coordinates
			y -= this->y;

			if ((x > 0) && (x < width) && (y > 0) && (y < height)) {		// Send event to children
				for (CONTAINER::iterator it = children.begin(); it != children.end(); it++) {
					(*it)->motion(x, y);									// Send event
				}
			}

			this->onMouseMove(x, y);
		}

		/* passiveMotion
			Capture passive mouse motion

			x, y - mouse coordinates
		*/
		bool GUIComponent::passiveMotion(int x, int y)
		{
			if (!enabled) return false;										// If not enabled do not intercept

			x -= this->x;													// Calculate local coordinates
			y -= this->y;

			bool intercepted = false;										// Send event to children

			for (CONTAINER::reverse_iterator it = children.rbegin(); it != children.rend(); ++it) {
				if (!intercepted) {											// Send event
					intercepted = (*it)->passiveMotion(x, y);
				}
				else {
					(*it)->mouseOut();
				}
			}

			if ((x >= 0) && (x < width) && (y >= 0) && (y < height)) {		// Did event occur inside component?
				if (!mouseover) {
					mouseover = true;
					this->onMouseOver();
				}
			}
			else {
				if (mouseover) this->onMouseOut();
				mouseover = false;
			}

			return mouseover;
		}

		void GUIComponent::mouseOut(void)
		{
			if (!mouseover) return;
			this->onMouseOut();
			mouseover = false;
		}

		/* key
			Capture key events and return true if event was intercepted

			ch - key pressed
			x, y - mouse coordinates
		*/
		bool GUIComponent::key(unsigned char ch, int x, int y)
		{
			if (!enabled) return false;
			x -= this->x;
			y -= this->y;

			// Send event to children
			for (CONTAINER::reverse_iterator it = children.rbegin(); it != children.rend(); ++it) {
				if ((*it)->key(ch, x, y)) {									// Send event and check status
					giveFocus(it);
					return true;
				}
			}

			return this->onKey(ch, x, y);
		}

		/* Capture special key events and return true if event was intercepted

			key - special key pressed
			x, y - mouse coordinates
		*/
		bool GUIComponent::special(int key, int x, int y)
		{
			if (!enabled) return false;
			x -= this->x;
			y -= this->y;

			// Send event to children
			for (CONTAINER::reverse_iterator it = children.rbegin(); it != children.rend(); ++it) {
				if ((*it)->special(key, x, y)) {							// Send event and check status
					giveFocus(it);
					return true;
				}
			}

			return this->onSpecial(key, x, y);
		}

		/* resize
			Called when GUIComponent should be resized

			w, h - new dimensions
		*/
		void GUIComponent::resize(int w, int h)
		{
			this->width = w;
			this->height = h;

			for (CONTAINER::iterator it = children.begin(); it != children.end(); ++it) {
				(*it)->parentResize(w, h);									// Send event
			}
			
			this->onResize();
		}

		/* parentResize
			Called when parent is resized

			w, h - new parent dimensions
		*/
		void GUIComponent::parentResize(int w, int h)
		{
			this->onParentResize(w, h);
		}

		/* giveFocus
			Gives the specified component focus if it wants it

			it - iterator pointing to the object to give focus to
		*/
		void GUIComponent::giveFocus(CONTAINER::reverse_iterator it)
		{
			if ((*it)->wantFocus()) {										// First ask if object wants focus?
				children.back()->lostFocus();								// Tell the object with focus it has lost it

				GUIComponent *component = *it;								// Give focus (the topmost object has focus)
				children.erase(--it.base());
				children.push_back(component);
				component->gotFocus();
			}
		}

		void GUIComponent::gotFocus(void)
		{
			if (!children.empty()) {
				if (children.back()->wantFocus()) {
					children.back()->gotFocus();
				}
			}
			hasfocus = true;
			this->onGotFocus();
		}

		void GUIComponent::lostFocus(void)
		{
			if (hasfocus) {
				hasfocus = false;
				this->onLostFocus();
				if (!children.empty()) {
					children.back()->lostFocus();
				}
			}
		}

		void GUIComponent::update(void)
		{
			for (CONTAINER::iterator it = children.begin(); it != children.end(); ++it) {
				(*it)->update();
			}
			onUpdate();
		}

		// onEvent methods
		void GUIComponent::onPaintBackground(int absx, int absy)
		{
		}

		void GUIComponent::onPaintOverlay(int absx, int absy)
		{
		}

		void GUIComponent::onMouseUp(int x, int y)
		{
		}

		void GUIComponent::onMouseDown(int x, int y)
		{
		}

		void GUIComponent::onRightMouseUp(int x, int y)
		{
		}

		void GUIComponent::onRightMouseDown(int x, int y)
		{
		}

		void GUIComponent::onMouseOver(void)
		{
		}

		void GUIComponent::onMouseOut(void) {
		}

		void GUIComponent::onMouseMove(int x, int y)
		{
		}

		void GUIComponent::onClick(int x, int y)
		{
		}

		void GUIComponent::onParentResize(int w, int h)
		{
		}

		void GUIComponent::onResize(void)
		{
		}

		void GUIComponent::onGotFocus(void)
		{
		}

		void GUIComponent::onLostFocus(void)
		{
		}

		bool GUIComponent::onKey(unsigned char ch, int x, int y)
		{
			return false;
		}

		bool GUIComponent::onSpecial(int key, int x, int y)
		{
			return false;
		}

		void GUIComponent::onUpdate(void)
		{
		}

	}
}