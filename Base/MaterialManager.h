#pragma once
#include <string>
#include <map>

using namespace std;

namespace Planet2
{
	class Material;
	class World;

	//Nacita a spravuje materialy ve formatu .mtl
	class MaterialManager
	{
	public:
		MaterialManager(World *world, const string &path);
		~MaterialManager(void);
		Material *GetMaterial(const string &mtl, const string &material);
	private:
		string _dir;
		World *_world;
		map<string, map<string, Material> > _data;
		void loadMTL(const string &mtl);
		Material *getMaterial(const string &mtl, const string &material);
		void initMaterial(Material *material);
	};
}

