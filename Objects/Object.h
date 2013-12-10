
#pragma once

#include "../GLee.h"
#include "../Base/Matrix.h"

#ifndef FLT_MAX
#define FLT_MAX             ((float)3.40282346638528860e+38)
#endif

class CROAMTriangle;

namespace Planet2 {

	class Object : public C3DObject {
	protected:
		Object() : C3DObject(), visible(true), rotateWithTriangle(false), tri(NULL), solid(false), managed(false) {}
	public:
		virtual void beginDraw() = 0;
		virtual void endDraw() = 0;
		virtual void update(C3DObject *camera, float dt) = 0;
		virtual void draw() = 0;
		bool visible;
		bool solid;
		bool rotateWithTriangle;
		bool managed;
		CROAMTriangle *tri;
	};

	struct ModelTextureGroup
	{
	public:
		GLuint VBO;
		GLuint Texture;
		ModelTextureGroup(GLuint vbo, GLuint texture) : VBO(vbo), Texture(texture){}
	};

	class GroupObject : public Object 
	{
	public:
		virtual ModelTextureGroup *getGroups() = 0;
		virtual int getGroupsCount() = 0;
		virtual void beginDrawGroup(ModelTextureGroup *group) = 0;
		virtual void endDrawGroup(ModelTextureGroup *group) = 0;
		virtual void update(C3DObject *camera, float dt) = 0;
		virtual void draw(ModelTextureGroup *group) = 0;

		virtual void beginDraw() {}
		virtual void endDraw() {}
		virtual void draw() {}
	};
}
