#pragma once
#include "Object.h"

#include <SDL/SDL_ttf.h>
#include <string>

#include "../GLee.h"

namespace Planet2
{

	class Screen;

	class Text : public Object
	{
	public:
		Text(TTF_Font *font, int x, int y, const std::string &text, int r, int g, int b);
		virtual ~Text();

		void beginDraw();
		void endDraw();
		void update(C3DObject *camera, float dt);
		void draw();

	private:
		GLuint _texture;
		int _x;
		int _y;
		int _w;
		int _h;
		int _vPort[4];
	};
}

