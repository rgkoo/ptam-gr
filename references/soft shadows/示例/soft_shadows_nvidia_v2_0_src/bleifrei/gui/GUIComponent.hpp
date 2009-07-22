
#ifndef _include_bleifrei_GUIComponent_hpp_
#define _include_bleifrei_GUIComponent_hpp_ 


// includes
#include <list>

namespace bleifrei {
	namespace gui {

		class GUIComponent {
		protected:
			typedef std::list<GUIComponent *> CONTAINER;

		public:
			enum MouseButton { LeftButton = 0, RightButton = 1, MiddleButton = 2 };
			enum MouseEvent { MouseDown, MouseUp };
			
			GUIComponent(void);
			virtual ~GUIComponent(void);

			void cleanup(void);

			void paint(int absx, int absy);
			void motion(int x, int y);
			void parentResize(int w, int h);
			void mouseOut(void);
			void gotFocus(void);
			void lostFocus(void);
			void update(void);

			bool passiveMotion(int x, int y);
			bool mouseDown(int button, int x, int y);
			void mouseUp(int button, int x, int y);
			bool key(unsigned char ch, int x, int y);
			bool special(int key, int x, int y);

			void move(int x, int y) {
				this->x = x;
				this->y = y;
			}
			void resize(int w, int h);

			void show(void)				{ visible = true; enable();					}
			void hide(void)				{ visible = false; disable();				}

			void enable(void)			{ enabled = true;							}
			void disable(void)			{ enabled = mouseover = clicked = false;	}

			bool isVisible(void) const	{ return visible;							}
			bool isEnabled(void) const	{ return enabled;							}

			bool isMouseOver(void) const{ return mouseover;							}
			bool isClicked(void) const	{ return clicked;							}

			bool wantFocus(void) const	{ return enabled && wantfocus && !hasfocus; }
			bool hasFocus(void) const	{ return hasfocus;							}

			int getX(void) const		{ return x;									}
			int getY(void) const		{ return y;									}

			int getWidth(void) const	{ return width;								}
			int getHeight(void) const	{ return height;							}

		protected:
			virtual void onPaintBackground(int absx, int absy);
			virtual void onPaintOverlay(int absx, int absy);
			virtual void onMouseUp(int x, int y);
			virtual void onMouseDown(int x, int y);
			virtual void onRightMouseUp(int x, int y);
			virtual void onRightMouseDown(int x, int y);
			virtual void onMouseOver();
			virtual void onMouseOut();
			virtual void onMouseMove(int x, int y);
			virtual void onClick(int x, int y);
			virtual void onParentResize(int w, int h);
			virtual void onResize();
			virtual void onGotFocus();
			virtual void onLostFocus();

			virtual bool onKey(unsigned char ch, int x, int y);
			virtual bool onSpecial(int key, int x, int y);

			virtual void onUpdate(void);

			void setWantFocus(bool wantfocus)		{ this->wantfocus = wantfocus;			}

			void giveFocus(CONTAINER::reverse_iterator it);

		private:
			int x, y;				/**< Component position relative to parent */
			int width, height;		/**< Component dimensions */

			bool visible;			/**< \c true if component is visible */
			bool enabled;			/**< \c true if component is enabled */
			bool clicked;			/**< \c true if mouse was pressed over the component */
			bool mouseover;			/**< \c true if the mouse is over the component */

			bool wantfocus;			/**< \c true if this component wants focus */
			bool hasfocus;			/**< \c true if this component has focus */

		protected:
			CONTAINER children;		/**< vector of child components */
		};

	}
}

#endif //_include_bleifrei_GUIComponent_hpp_
