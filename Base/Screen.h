
#pragma once

#include <cassert>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "../GLee.h"

#include <SDL/SDL.h>

#include "../Objects/Planet.h"
#include "World.h"

#include "Matrix.h"

namespace Planet2 {

	class Planet;

	class Screen : public World
	{
	public:
		Screen(unsigned width, unsigned height, unsigned color, unsigned depth, unsigned stencil);
		virtual ~Screen();

		CPlanet *getPlanet() {			
			return Planet;
		}

	protected:
		virtual void onInit();
		virtual void onWindowRedraw(float dt);
		virtual void onWindowResized(int w, int h);

	private:
		unsigned _color;
		SDL_Surface *_screen;
		int _width;
		int _height;
		float _rx;
		float _ry;
		float _pz;

		friend class MainLoop;
	};

}
