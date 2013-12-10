
#include "SkyBox.h"
#include "../pgr.h"
#include "../Base/World.h"

#include "GL/glu.h"
#include <SDL/SDL_image.h>

using namespace Planet2;

#define BACK_ID		0
#define FRONT_ID	1
#define TOP_ID		2
#define BOTTOM_ID	3
#define RIGHT_ID	4
#define LEFT_ID		5

SkyBox::SkyBox(World *world) : _world(world)
{	
	loadTexture("skybox_2_top3.jpg", BOTTOM_ID);
	loadTexture("skybox_2_bottom4.jpg", TOP_ID);
	loadTexture("skybox_2_left2.jpg", RIGHT_ID);
	loadTexture("skybox_2_right1.jpg", LEFT_ID);
	loadTexture("skybox_2_front5.jpg", FRONT_ID);
	loadTexture("skybox_2_back6.jpg", BACK_ID);

	// 	loadTexture("test.png", BOTTOM_ID);
	// 	loadTexture("test.png", TOP_ID);
	// 	loadTexture("test.png", RIGHT_ID);
	// 	loadTexture("test.png", LEFT_ID);
	// 	loadTexture("test.png", FRONT_ID);
	// 	loadTexture("test.png", BACK_ID);
}

SkyBox::~SkyBox() {

}

void SkyBox::loadTexture(const char *file, int id) 
{
	_textures[id] = _world->Textures->GetTexture(file);

	/*SDL_Surface * surface = IMG_Load(file);	
	if(surface == NULL) 
	throw SDL_Exception();

	glGenTextures(1, &_textures[id]);
	glBindTexture(GL_TEXTURE_2D, _textures[id]);
	SurfaceImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_LINEAR_MIPMAP_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(surface);*/
}

void SkyBox::drawSkyBox(float x, float y, float z, float width, float height, float length) {
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

	glBindTexture(GL_TEXTURE_2D, _textures[BACK_ID]);
	glBegin(GL_QUADS);		
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z);		
	glEnd();

	glBindTexture(GL_TEXTURE_2D, _textures[FRONT_ID]);
	glBegin(GL_QUADS);	
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z + length);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, _textures[BOTTOM_ID]);
	glBegin(GL_QUADS);		

	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y,			z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,			z + length); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, _textures[TOP_ID]);
	glBegin(GL_QUADS);		
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y + height,	z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, _textures[LEFT_ID]);
	glBegin(GL_QUADS);		
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);	
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height,	z + length); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);			
	glEnd();

	glBindTexture(GL_TEXTURE_2D, _textures[RIGHT_ID]);
	glBegin(GL_QUADS);	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,	z + length); 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,	z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void SkyBox::draw() 
{
	drawSkyBox(0, 0, 0, 8000, 8000, 8000);
}

void SkyBox::update(C3DObject *camera, float dt) {}

void SkyBox::beginDraw()
{
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);	
}

void SkyBox::endDraw()
{
	glPopAttrib();
}

