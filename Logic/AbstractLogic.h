
#pragma once

#include <cassert>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include <SDL/SDL.h>

namespace Planet2 {

class AbstractLogic {
	public:
		virtual void onKeyDown(SDLKey key, Uint16 /*mod*/) = 0;
		virtual void onKeyUp(SDLKey /*key*/, Uint16 /*mod*/) = 0;
		virtual void onMouseMove(unsigned /*x*/, unsigned /*y*/, int xrel, int yrel, Uint8 buttons) = 0;
		virtual void onMouseDown(Uint8 button, unsigned /*x*/, unsigned /*y*/) = 0;
		virtual void onMouseUp(Uint8 /*button*/, unsigned /*x*/, unsigned /*y*/) = 0;
		virtual void onTick() = 0;
		virtual float getDT() = 0;
};

}
