
#pragma once

#include "../GLee.h"

#include <SDL/SDL.h>

#include "../Base/Matrix.h"

#include "Object.h"

namespace Planet2 {

	class World;

	class SkyBox : public Object {
	public:
		SkyBox(World *world);
		virtual ~SkyBox();

		void draw();
		void update(C3DObject *camera, float dt);
		void beginDraw();
		void endDraw();

	private:
		void drawSkyBox(float x, float y, float z, float width, float height, float length);
		void loadTexture(const char *file, int id);

	private:
		GLuint _textures[6];
		World *_world;

	};


}
