
#include "MainLoop.h"
#include "Screen.h"
#include "../Logic/GameLogic.h"
#include "../pgr.h"

namespace Planet2 {

MainLoop::MainLoop(Screen *screen, AbstractLogic *logic, unsigned period) :
_screen(screen), _logic(logic), _period(period) {
}

MainLoop::~MainLoop() {

}


void MainLoop::mainloop() {
	// Window is not minimized
	bool active = true;

	for(;;)// Infinite loop
	{
		SDL_Event event;

		// Wait for event
		if(SDL_WaitEvent(&event) == 0) throw SDL_Exception();

		// Screen needs redraw
		bool redraw = false;

		// Handle all waiting events
		do
		{
			// Call proper event handlers
			switch(event.type)
			{
			case SDL_ACTIVEEVENT :// Stop redraw when minimized
				if(event.active.state == SDL_APPACTIVE)
					active = event.active.gain;
				break;
			case SDL_KEYDOWN :
				_logic->onKeyDown(event.key.keysym.sym, event.key.keysym.mod);
				break;
			case SDL_KEYUP :
				_logic->onKeyUp(event.key.keysym.sym, event.key.keysym.mod);
				break;
			case SDL_MOUSEMOTION :
				_logic->onMouseMove(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel, event.motion.state);
				break;
			case SDL_MOUSEBUTTONDOWN :
				_logic->onMouseDown(event.button.button, event.button.x, event.button.y);
				break;
			case SDL_MOUSEBUTTONUP :
				_logic->onMouseUp(event.button.button, event.button.x, event.button.y);
				break;
			case SDL_QUIT :
				return;// End main loop
			case SDL_VIDEORESIZE :
				_screen->onWindowResized(event.resize.w, event.resize.h);
				break;
			case SDL_VIDEOEXPOSE :
				redraw = true;
				break;
			default :// Do nothing
				break;
			}
		} while(SDL_PollEvent(&event) == 1);

		

		// Optionally redraw window
		if(active && redraw) {
			_logic->onTick();
			_screen->onWindowRedraw(_logic->getDT());
		}
	}
}

void MainLoop::run() {
	// This main loop requires timer support
	if(SDL_InitSubSystem(SDL_INIT_TIMER) < 0) throw SDL_Exception();

	// Create redraw timer
	class RedrawTimer
	{
	private :
		SDL_TimerID id;
		static Uint32 callback(Uint32 interval, void *)
		{
			redraw();
			return interval;
		}
	public :
		RedrawTimer(unsigned interval)
			: id(SDL_AddTimer(interval, callback, NULL))
		{
			if(id == NULL) throw SDL_Exception();
		}
		~RedrawTimer()
		{
			if(id != NULL) SDL_RemoveTimer(id);
		}
	} redrawTimer(_period);

	// Start simple main loop
	mainloop();
}

}