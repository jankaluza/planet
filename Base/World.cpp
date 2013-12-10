#include "World.h"
#include "Matrix.h"
#include "ObjectManager.h"
#include "FontManager.h"

using namespace Planet2;

World::World()
{
	Camera = new C3DObject();
	Textures = new TextureManager("./data/textures/");
	Materials = new MaterialManager(this, "./data/materials/");
	Models = new ModelManager(this, "./data/models/");
	Objects = new ObjectManager(this, 5);
	Fonts = new FontManager("./data/fonts/");
}

World::~World()
{
	delete Camera;
	delete Textures;
	delete Materials;
	delete Models;
	delete Objects;
	delete Fonts;
}

