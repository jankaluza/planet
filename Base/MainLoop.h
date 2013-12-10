
#pragma once

#include <cassert>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include <SDL/SDL.h>

#include "../GLee.h"

namespace Planet2 {

class Screen;
class AbstractLogic;

class MainLoop {
	public:
		MainLoop(Screen *screen, AbstractLogic *logic, unsigned period);
		virtual ~MainLoop();

		void run();

	private:
		void mainloop();

	private:
		Screen *_screen;
		AbstractLogic *_logic;
		unsigned _period;
};

}
