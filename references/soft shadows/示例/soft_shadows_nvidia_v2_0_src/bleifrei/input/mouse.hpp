
#ifndef _include_bleifrei_mouse_hpp_
#define _include_bleifrei_mouse_hpp_ 


namespace bleifrei {
	namespace input {
		namespace mouse {

			enum Mode {
				Relative,
				Absolute
			};

			void setMode(Mode mode);
			void getMovement(int &x, int &y);
			int getMovementX(void);
			int getMovementY(void);

			void getPosition(int &x, int &y);
			int getPositionX(void);
			int getPositionY(void);

			enum Button {
				Left,
				Right,
				Middle
			};

			bool isDown(int button);

			void update(void);

		}
	}
}


#endif //_include_bleifrei_keyboard_hpp_
