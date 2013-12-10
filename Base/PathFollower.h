
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

class PathFollower {
	public:
		PathFollower(World *world, const Path &path);
		virtual ~PathFollower();

		void reset();
		bool follow(float dt, CVector &pos, int &turnPreparation);
		void setSpeed(float speed) {
			_speed = speed;
		}

	private:
		Path _path;
		int _currentPoint;
		float _lat;
		float _lon;
		World *_world;
		float _speed;
};

}
