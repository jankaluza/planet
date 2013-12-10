#pragma once

#include "Object.h"

namespace Planet2
{
	class Model;
	class World;

	class Cat : public Object
	{
	public:
		Cat(World *world);

		void beginDraw();
		void endDraw();
		void update(C3DObject *camera, float dt);
		void draw();

	private:
		Model *_model;
		World *_world;
		CMatrix _m;
	};
}

