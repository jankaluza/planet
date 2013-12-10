
#pragma once

#include <cassert>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include <SDL/SDL.h>

#include "AbstractLogic.h"
#include "../Base/Matrix.h"

class CROAMTriangle;

namespace Planet2 {

class Screen;
class PathFollower;
class Text;

class GameLogic : public AbstractLogic {
	public:
		typedef enum { PreIntro, Intro, Game, GameOver } GameState;
		GameLogic(Screen *screen);
		virtual ~GameLogic();

		void onKeyDown(SDLKey key, Uint16 /*mod*/);
		void onKeyUp(SDLKey /*key*/, Uint16 /*mod*/);
		void onMouseMove(unsigned /*x*/, unsigned /*y*/, int xrel, int yrel, Uint8 buttons);
		void onMouseDown(Uint8 button, unsigned /*x*/, unsigned /*y*/);
		void onMouseUp(Uint8 /*button*/, unsigned /*x*/, unsigned /*y*/);
		void onTick();
		float getDT() { return _dt; }

		void reset();

	private:
		bool detectCollision();
		void handleIntro(float dt);
		void handleGame(float dt);
		void handleGameOver(float dt);

	private:
		Screen *_screen;
		float _rx;
		float _ry;
		PathFollower *_follower;
		float _t0;
		float _t1;
		float _height;
		float _toHeight;
		float _sumT;
		int _ticksPerSec;
		float _jumpHeight;
		float _dt;
		float _camFlyAngle;

		int _cameraHeight;
		int _cameraDistance;
		GameState _state;
		CVector _last;

		Text *_three;
		Text *_two;
		Text *_one;
		Text *_go;
		Text *_gameOver;
		Text *_left;
		Text *_right;
		Text *_press;
		Text *_l1;
		Text *_l2;
		Text *_l3;
		Text *_win;
};

}
