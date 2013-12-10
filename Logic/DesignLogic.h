
#pragma once

#include <cassert>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include <SDL/SDL.h>

#include "AbstractLogic.h"

namespace Planet2 {

class Screen;
class PathFollower;

class DesignLogic : public AbstractLogic {
	public:
		DesignLogic(Screen *screen);
		virtual ~DesignLogic();

		void onKeyDown(SDLKey key, Uint16 /*mod*/);
		void onKeyUp(SDLKey /*key*/, Uint16 /*mod*/);
		void onMouseMove(unsigned /*x*/, unsigned /*y*/, int xrel, int yrel, Uint8 buttons);
		void onMouseDown(Uint8 button, unsigned /*x*/, unsigned /*y*/);
		void onMouseUp(Uint8 /*button*/, unsigned /*x*/, unsigned /*y*/);
		void onTick();
		float getDT() { return _dt; }

	private:
		enum ForwardMode
		{
			FM_None,
			FM_Forward,
			FM_Backward
		};

		enum SideMode
		{
			SM_None,
			SM_Left,
			SM_Right
		};

		enum RotateMode
		{
			RM_None,
			RM_CCW,
			RM_CW
		};

		Screen *_screen;
		float _rx;
		float _ry;
		PathFollower *_follower;
		float _t0;
		float _t1;
		float _dt;

		float _sumT;
		int _ticksPerSec;

		ForwardMode _fm;
		SideMode _sm;
		RotateMode _rm;
};

}
