#include "pgr.h"

#include "Base/Screen.h"
#include "Base/MainLoop.h"
#include "Logic/GameLogic.h"
#include "Logic/DesignLogic.h"
#include <SDL/SDL_ttf.h>

using namespace std;

int main(int argc, char ** argv)
{
	//srand((unsigned)time(0));
	try {
		// Init SDL - only video subsystem will be used
		if(SDL_Init(SDL_INIT_VIDEO) < 0) throw SDL_Exception();

		// Shutdown SDL when program ends
		atexit(SDL_Quit);

		TTF_Init();
		atexit(TTF_Quit);

		Planet2::Screen scr(800, 600, 24, 24, 0);
		Planet2::AbstractLogic *logic = 0;
		if (argc == 2 && std::string(argv[1]) == "design") {
			logic = new Planet2::DesignLogic(&scr);
		}
		else {
			logic = new Planet2::GameLogic(&scr);
		}
		Planet2::MainLoop loop(&scr, logic, 16);
		loop.run();
		delete logic;

	} catch(exception & ex) {
		cout << "ERROR : " << ex.what() << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
