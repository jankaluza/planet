#include "TextureManager.h"
#include "../pgr.h"
#include <SDL/SDL_image.h>

using namespace Planet2;

TextureManager::TextureManager(const string &path) : _dir(getFilePath(path))
{
}


TextureManager::~TextureManager(void)
{
	
}

GLuint TextureManager::GetTexture(const string &name)
{
	map<string, GLuint>::iterator it = _data.find(name);
	if (it != _data.end())
		return it->second;

	return _data[name] = loadTexture(name);
}

GLuint TextureManager::loadTexture(const string &name)
{
	string path = _dir + name;
	SDL_Surface * surface = IMG_Load(path.c_str());
	if(surface == NULL) 
		throw SDL_Exception();
	
	GLuint ptr;

	glGenTextures(1, &ptr);
	glBindTexture(GL_TEXTURE_2D, ptr);
	SurfaceImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_LINEAR_MIPMAP_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(surface);

	return ptr;
}
