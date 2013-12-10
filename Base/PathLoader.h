
#pragma once

#include <cassert>

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <vector>

#include <SDL/SDL.h>

#include "../GLee.h"
#include "Matrix.h"
#include "World.h"

namespace Planet2 {

class CPlanet;

class PathLoader {
	public:
		PathLoader(const std::string &file);
		virtual ~PathLoader();

		bool load(World *world);

	private:
		std::string _file;
};

}
