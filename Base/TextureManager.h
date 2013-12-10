#pragma once

#include "../GLee.h"
#include <string>
#include <map>

using namespace std;
namespace Planet2
{
	//Vsechny textury nacita a uvolnuje tato trida
	class TextureManager
	{
	public:
		TextureManager(const string &path);
		~TextureManager(void);

		GLuint GetTexture(const string &name);
	private:
		string _dir;
		map<string, GLuint> _data;
		GLuint loadTexture(const string &name);
	};
}

