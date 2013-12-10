#pragma once

#include "MaterialManager.h"
#include "TextureManager.h"
#include "ModelManager.h"

class C3DObject;

class PathNode {
public:
	PathNode(float lat, float lon, int dir) : lat(lat), lon(lon), dir(dir) {}
	float lat;
	float lon;
	int dir;
	static float Distance(const PathNode &a, const PathNode &b, float r)
	{		
		float x = (b.lon-a.lon) * CMath::cos((a.lat+b.lat)/2);
		float y = (b.lat-a.lat);
		return CMath::sqrt(x*x + y*y) * r;
	}
	CVector ToVector()
	{
		return CVectorFromSpherical(lat, lon);
	}
};

typedef std::vector<PathNode> Path;

namespace Planet2
{
	class ObjectManager;
	class FontManager;
	class CPlanet;
	class Hero;
	class Cat;

	class World
	{
	public:
		C3DObject *Camera;
		TextureManager *Textures;
		MaterialManager *Materials;
		ModelManager *Models;
		ObjectManager *Objects;
		CPlanet *Planet;
		Hero *Heron;
		Cat *Catn;
		FontManager *Fonts;
		Path path;

		World();
		virtual ~World();
	};
}

