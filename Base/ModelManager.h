#pragma once

#include <vector>
#include <map>
#include "Matrix.h"
#include <stdint.h>
#include <string.h>
#include <stdexcept>
#include <iostream>
#include "../GLee.h"

using namespace std;

namespace Planet2
{
	/*! Three-index vertex, indexing start at 0, -1 means invalid vertex. */
	struct IndVertex {
		int v, vt, vn;
		IndVertex() {};
		IndVertex(int v) : v(v), vt(v), vn(v) {};
		IndVertex(int v, int vt, int vn) : v(v), vt(vt), vn(vn) {};
	};

	/*! \struct Material
	*  \brief a simple structure to store material's properties
	*/
	class Material
	{
	public:
		CVector Ka, Kd, Ks;   /*! ambient, diffuse and specular rgb coefficients */
		float d;            /*! transparency */
		float Ns, Ni;       /*! specular exponent and index of refraction */
		string texDiffuse;
		GLuint glTexDiffuse;
	};

	struct Vec2f 
	{
		float x;
		float y;

		Vec2f() : x(0), y(0) {};
		Vec2f(float x, float y) : x(x), y(y) {};
	};

	struct Vertex
	{
		float location[3];
		float normal[3];
		float tex[2];
	};

	struct Primitive
	{
		Primitive(int offset, int numVertices, Vertex *vertices, Material *material) 
			: offset(offset), numVertices(numVertices), vertices(vertices), material(material) {}

		//Primitiva jednoho modelu nyni sdili stejny VBO
		const Vertex *vertices;
		int offset; //odsazeni od zacatku VBO
		int numVertices; //pocet vrcholu k vykresleni
		GLuint vbo;
		const Material *material;

		/*void drawSingle()
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, tex));
			glNormalPointer(GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, normal));
			glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, location)); //Vertex pointer az nakonec

			drawCore();

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_ARRAY_BUFFER);

			glBindTexture(GL_TEXTURE_2D, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void drawCore()
		{
			glBindTexture(GL_TEXTURE_2D, material->glTexDiffuse);
			glDrawArrays(GL_TRIANGLES, offset, numVertices);
		}

		void initialize()
		{
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}*/
	};

	static inline bool operator < ( const IndVertex& a, const IndVertex& b ) {
		if (a.v  != b.v)  return a.v  < b.v;
		if (a.vn != b.vn) return a.vn < b.vn;
		if (a.vt != b.vt) return a.vt < b.vt;
		return false;
	}

	/*! Fill space at the end of the token with 0s. */
	static inline const char* trimEnd(const char* token) {
		size_t len = strlen(token);
		if (len == 0) return token;
		char* pe = (char*)(token + len - 1);
		while ((*pe == ' ' || *pe == '\t' || *pe == '\r') && pe >= token) *pe-- = 0;
		return token;
	}

	/*! Determine if character is a separator. */
	static inline bool isSep(const char c) {
		return (c == ' ') || (c == '\t');
	}

	/*! Parse separator. */
	static inline const char* parseSep(const char*& token) {
		size_t sep = strspn(token, " \t");
		if (!sep) throw std::runtime_error("separator expected");
		return token+=sep;
	}

	/*! Parse optional separator. */
	static inline const char* parseSepOpt(const char*& token) {
		return token+=strspn(token, " \t");
	}

	/*! Read float from a string. */
	static inline float getFloat(const char* &token) {
		token += strspn(token, " \t");
		float n = (float)atof(token);
		token += strcspn(token, " \t\r");
		return n;
	}

	/*! Read Vec2f from a string. */
	static inline Vec2f getVec2f(const char* &token) {
		float x = getFloat(token);
		float y = getFloat(token);
		return Vec2f(x, y);
	}

	/*! Read Vec3f from a string. */
	static inline CVector getVec3f(const char* &token) {
		float x = getFloat(token);
		float y = getFloat(token);
		float z = getFloat(token);
		return CVector(x,y,z);
	}

	class World;

	class Model
	{
	public:
		vector<Primitive> Primitives;
		//void drawSimple()
		//{
		//	//Vykresleni vsech primitiv pomoci jednoho VBO
		//	{
		//		beginDraw();			

		//		for (size_t i = 0; i < Primitives.size(); i++)	
		//		{
		//			Primitives[i].drawCore();
		//		}

		//		endDraw();
		//	}


		//	//Vykresleni vsech primitiv
		//	{
		//		/*for (size_t i = 0; i < Primitives.size(); i++)	
		//		{
		//		Primitives[i].drawSingle();
		//		}*/
		//	}

		//	//Iterator je z nejakeho duvodu pomalejsi nez indexovani
		//	{
		//		/*for (vector<Primitive>::iterator it = Primitives.begin(); it != Primitives.end(); ++it) 
		//		{
		//		it->drawSimple();
		//		}*/
		//	}
		//}
		void mergePrimitives();
	protected:
		//void beginDraw()
		//{
		//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//	glEnableClientState(GL_NORMAL_ARRAY);
		//	glEnableClientState(GL_VERTEX_ARRAY);

		//	glBindBuffer(GL_ARRAY_BUFFER, Primitives[0].vbo);

		//	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, tex));
		//	glNormalPointer(GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, normal));
		//	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, location)); //Vertex pointer az nakonec

		//}
		//void endDraw()
		//{
		//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//	glDisableClientState(GL_NORMAL_ARRAY);
		//	glDisableClientState(GL_ARRAY_BUFFER);

		//	glBindTexture(GL_TEXTURE_2D, 0);
		//	glBindBuffer(GL_ARRAY_BUFFER, 0);
		//}
	};

	class ModelManager
	{
	public:
		ModelManager(World *world, const string &path);
		Model *GetModel(const string &name);

	private:
		IndVertex getInt3(const char*& token);
		int fix_v(int index) { return(index > 0 ? index - 1 : (index == 0 ? 0 : (int)_v .size() + index)); }
		int fix_vt(int index) { return(index > 0 ? index - 1 : (index == 0 ? 0 : (int)_vt.size() + index)); }
		int fix_vn(int index) { return(index > 0 ? index - 1 : (index == 0 ? 0 : (int)_vn.size() + index)); }
		vector<CVector> _v, _vn;
		vector<Vec2f> _vt;
		vector<vector<IndVertex> > _curGroup;
		Model *_curModel;
		Material* _curMaterial;
		World *_world;
		map<string, Model> _data;
		string _dir;
		void loadModel(const string &name);
		void clear();
		void flushFaceGroup();
		void addVertex(vector<CVector>&, vector<CVector>&, vector<Vec2f>&, const IndVertex&);
	};
}

