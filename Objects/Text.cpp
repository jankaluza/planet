#include "Text.h"
#include <iostream>

using namespace Planet2;

Text::Text(TTF_Font *font, int x, int y, const std::string &text, int r, int g, int b) :
	_x(x), _y(y) {
	SDL_Color color;
	SDL_Surface * surfa;
	SDL_Surface * surfb;
	GLuint texture;

	// Prepare SDL_Surface
	color.r = r;
	color.g = g;
	color.b = b;
	surfa = TTF_RenderUTF8_Blended(font, text.c_str(), color);
	surfb = SDL_CreateRGBSurface(0, surfa->w, surfa->h, 32, 0,0,0,0);
	SDL_BlitSurface(surfa, 0, surfb, 0);

	// Create GL texture
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surfa->w, surfa->h, 0, GL_BGRA, 
			GL_UNSIGNED_BYTE, surfb->pixels );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	_w = surfa->w;
	_h = surfa->h;

	/* Clean up */
	SDL_FreeSurface(surfa);
	SDL_FreeSurface(surfb);
}

Text::~Text() {
	glDeleteTextures(1, &_texture);
}

void Text::update(C3DObject *pCamera, float dt)
{
}

void Text::beginDraw()
{
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);

   // Enable 2D
	glGetIntegerv(GL_VIEWPORT, _vPort);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, _vPort[2], 0, _vPort[3], -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}

void Text::endDraw()
{
	// Disable 2D
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();   
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glPopAttrib();
}

void Text::draw()
{
	glBindTexture(GL_TEXTURE_2D, _texture);
	glColor3d(1.0, 1.0, 1.0);

	int x = _x;
	int y = _y;

	if (_x == -1) {
		x = (_vPort[2] / 2) - (_w / 2);
	}
	if (_y == -1) {
		y = (_vPort[3] / 2) - (_h / 2);
	}
	if (_y == -500) {
		y = (_vPort[3]) - (_h) + 20;
	}

	glBegin(GL_QUADS);
	glTexCoord2d(0, 1); glVertex2d(x,      y     );
	glTexCoord2d(1, 1); glVertex2d(x + _w, y     );
	glTexCoord2d(1, 0); glVertex2d(x + _w, y + _h);
	glTexCoord2d(0, 0); glVertex2d(x,      y + _h);
	glEnd();
}
