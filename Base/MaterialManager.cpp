#include "MaterialManager.h"
#include "../pgr.h"
#include "World.h"
#include <fstream>

using namespace Planet2;

MaterialManager::MaterialManager(World *world, const string &path) : _world(world), _dir(getFilePath(path))
{

}

MaterialManager::~MaterialManager(void)
{

}

Material *MaterialManager::getMaterial(const string &mtl, const string &material)
{
	map<string, map<string, Material> >::iterator it = _data.find(mtl);
	if (it != _data.end())
	{
		map<string, Material>::iterator it2 = it->second.find(material);

		if (it2 == it->second.end())
			throw new std::runtime_error("unknown material");

		return &it2->second;
	}
	return NULL;
}

Material *MaterialManager::GetMaterial(const string &mtl, const string &material)
{
	Material *mat = getMaterial(mtl, material);

	if (mat != NULL)
		return mat;

	loadMTL(mtl);
	return getMaterial(mtl, material);
}

void MaterialManager::loadMTL(const string &mtl)
{
	string filename = _dir + mtl;

	ifstream ifs;	
	ifs.open(filename.c_str());
	if (!ifs.is_open())
	{
		throw std::exception();
	}
	Material* mat = NULL;
	std::cout << "'" << filename << "'\n";

	while (ifs.peek() != EOF) 
	{
		char line[MAX_LINE_LENGTH];
		ifs.getline(line, sizeof(line), '\n');

		const char* token = line + strspn(line, " \t"); // ignore spaces and tabs
		if (token[0] == 0 || token[0] == '\r' || token[0] == '\n') continue; // ignore empty lines
		if (token[0] == '#') continue; // ignore comments
		if (!strncasecmp(token, "newmtl", 6)) {
			parseSep(token += 6);
			std::string name(trimEnd(token));

			_data[mtl][name] = Material();
			mat = &_data[mtl][name];
			continue;
		}

		if (!mat) throw std::runtime_error("invalid material file: newmtl expected first");

		if (!strncasecmp(token, "d", 1)) { parseSep(token += 1); trimEnd(token); mat->d = getFloat(token); continue; }
		if (!strncasecmp(token, "Ns", 1)) { parseSep(token += 2); trimEnd(token); mat->Ns = getFloat(token); continue; }
		if (!strncasecmp(token, "Ns", 1)) { parseSep(token += 2); trimEnd(token); mat->Ni = getFloat(token); continue; }
		if (!strncasecmp(token, "Ka", 2)) { parseSep(token += 2); trimEnd(token); mat->Ka = getVec3f(token); continue; }
		if (!strncasecmp(token, "Kd", 2)) { parseSep(token += 2); trimEnd(token); mat->Kd = getVec3f(token); continue; }
		if (!strncasecmp(token, "Ks", 2)) { parseSep(token += 2); trimEnd(token); mat->Ks = getVec3f(token); continue; }
		if (!strncasecmp(token, "map_Kd", 6)) { parseSep(token += 6); mat->texDiffuse = string(trimEnd(token)); continue; }
	}
	ifs.close();

	//Inicializace všech textur v novì naètených materiálech
	for (map<string, Material>::iterator it = _data[mtl].begin(); it != _data[mtl].end(); it++)
	{
		initMaterial(&it->second);
	}
}

void MaterialManager::initMaterial(Material *material)
{
	if (!material->texDiffuse.empty())
		material->glTexDiffuse = _world->Textures->GetTexture(material->texDiffuse);
}
