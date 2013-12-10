
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

class MapGenerator {
	public:
		MapGenerator();
		virtual ~MapGenerator();

		void generate(World *world);

	private:
		void generatePlants(World *world);
		void generateCactis(World *world);

	private:
		bool isValidPosition(World *world, float lat, float lon);
		std::vector<CVector> points;
};

}
