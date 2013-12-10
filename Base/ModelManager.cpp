#include "ModelManager.h"
#include <fstream>
#include <string>
#include "../pgr.h"
#include "World.h"

using namespace Planet2;

ModelManager::ModelManager(World *world, const string &path) : _world(world), _dir(getFilePath(path))
{

}

Model *ModelManager::GetModel(const string &name)
{
	map<string, Model>::iterator it = _data.find(name);
	if (it != _data.end())
		return &it->second;

	loadModel(name);

	return &_data[name];
}

/*! Parse differently formated triplets like: n0, n0/n1/n2, n0//n2, n0/n1.          */
/*! All indices are converted to C-style (from 0). Missing entries are assigned -1. */
IndVertex ModelManager::getInt3(const char*& token)
{
	IndVertex v(-1);
	v.v = fix_v(atoi(token));
	token += strcspn(token, "/ \t\r");
	if (token[0] != '/') return(v);
	token++;

	// it is i//n
	if (token[0] == '/') {
		token++;
		v.vn = fix_vn(atoi(token));
		token += strcspn(token, " \t\r");
		return(v);
	}

	// it is i/t/n or i/t
	v.vt = fix_vt(atoi(token));
	token += strcspn(token, "/ \t\r");
	if (token[0] != '/') return(v);
	token++;

	// it is i/t/n
	v.vn = fix_vn(atoi(token));
	token += strcspn(token, " \t\r");
	return(v);
}

void ModelManager::flushFaceGroup()
{
	if (_curGroup.empty()) return;

	// temporary data arrays
	vector<CVector> positions;
	vector<CVector> normals;
	vector<Vec2f> texcoords;

	// merge three indices into one
	for (size_t j = 0; j < _curGroup.size(); j++)
	{
		/* iterate over all faces */
		const vector<IndVertex>& face = _curGroup[j];
		IndVertex i0 = face[0], i1 = IndVertex(-1), i2 = face[1];

		/* triangulate the face with a triangle fan */
		for (size_t k = 2; k < face.size(); k++) {
			i1 = i2; i2 = face[k];
			addVertex(positions, normals, texcoords, i0);
			addVertex(positions, normals, texcoords, i1);
			addVertex(positions, normals, texcoords, i2);
		}
	}
	_curGroup.clear();

	if (normals.size() && normals.size() != positions.size()) 
		throw runtime_error("invalid count of normals");
	if (texcoords.size() && texcoords.size() != positions.size())
		throw runtime_error("invalid count of texcoords");

	//Data pro VBO
	{
		Vertex *vertices = new Vertex[positions.size()];

		for (size_t i = 0; i < positions.size(); i++)
		{
			Vertex *ver = &vertices[i];
			CVector position = positions[i];

			ver->location[0] = position.x;
			ver->location[1] = position.y;
			ver->location[2] = position.z;

			if (normals.size())
			{
				CVector normal = normals[i];

				ver->normal[0] = normal.x;
				ver->normal[1] = normal.y;
				ver->normal[2] = normal.z;
			}

			if (texcoords.size())
			{
				Vec2f tex = texcoords[i];

				ver->tex[0] = tex.x;
				ver->tex[1] = tex.y;
			}
		}

		_curModel->Primitives.push_back(Primitive(0, positions.size(), vertices, _curMaterial));
	}
}

void ModelManager::addVertex(vector<CVector> &positions, vector<CVector> &normals, vector<Vec2f> &texcoords, const IndVertex &i)
{
	positions.push_back(_v[i.v]);
	if (i.vn >= 0) normals.push_back(_vn[i.vn]);
	if (i.vt >= 0) texcoords.push_back(_vt[i.vt]);
}

void ModelManager::loadModel(const string &name)
{
	string filename = _dir + name;

	ifstream ifs;
	ifs.open(filename.c_str());
	if (ifs.fail()) 
		throw std::runtime_error("can't open file " + std::string(filename));

	_curMaterial = NULL;
	_curModel = &_data[name];

	char line[MAX_LINE_LENGTH]; 
	string mtl;

	while (ifs.peek() != EOF)
	{
		ifs.getline(line, sizeof(line), '\n');
		const char* token = line + strspn(line, " \t"); // ignore space and tabs

		if (token[0] == 0) continue;
		if (token[0] == 'v' && isSep(token[1])) { _v.push_back(getVec3f(token += 2)); continue; }
		if (!strncasecmp(token, "vn",  2) && isSep(token[2])) { _vn.push_back(getVec3f(token += 3)); continue; }
		if (!strncasecmp(token, "vt",  2) && isSep(token[2])) { _vt.push_back(getVec2f(token += 3)); continue; }
		if (token[0] == 'f' && isSep(token[1])) {
			parseSep(token += 1);
			std::vector<IndVertex> face;
			while (token[0] && token[0] != '\r' && token[0] != '\n') {
				face.push_back(getInt3(token));
				parseSepOpt(token);
			}
			_curGroup.push_back(face);
			continue;
		}

		/*! use material */
		if (!strncasecmp(token, "usemtl", 6) && isSep(token[6]))
		{
			if (mtl.empty())
				throw new std::runtime_error("mtllib not specified");

			flushFaceGroup();
			std::string name(trimEnd(parseSep(token += 6)));

			_curMaterial = _world->Materials->GetMaterial(mtl, name);
			continue;
		}

		/* load material library */
		if (!strncasecmp(token, "mtllib", 6) && isSep(token[6])) 
		{
			mtl = string(trimEnd(parseSep(token += 6)));
			continue;
		}
	}
	//Last 
	flushFaceGroup();

	ifs.close();

	//Slouceni primitiv pro spolecne VBO
	_curModel->mergePrimitives();

	//Inicializace VBOs
	/*for (size_t i = 0; i < _curModel->Primitives.size(); i++)
	{
		_curModel->Primitives[i].initialize();
	}*/

	clear();
}

void Model::mergePrimitives()
{
	int totalSize = 0;

	//Inicializace VBOs
	for (size_t i = 0; i < Primitives.size(); i++)
	{
		totalSize += Primitives[i].numVertices;
	}

	Vertex *vertices = new Vertex[totalSize];
	size_t index = 0;
	for (size_t i = 0; i < Primitives.size(); i++)
	{
		Primitive *cur = &Primitives[i];
		cur->offset = index;

		for (size_t j = 0; j < cur->numVertices; j++)
		{
			//toto je asi spatne ... potrebuju to fyzicky prekopirovat, nejen priradit
			vertices[index] = Vertex(cur->vertices[j]);
			index++;
		}		
		//delete cur->vertices;
		cur->vertices = vertices;
	}

	//Vytvoreni VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, totalSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for (size_t i = 0; i < Primitives.size(); i++)
	{
		Primitives[i].vbo = vbo;
	}
}

void ModelManager::clear()
{
	_curGroup.clear();
	_v.clear();
	_vn.clear();
	_vt.clear();
	_curModel = NULL;
	_curMaterial = NULL;
}
