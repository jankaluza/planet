#pragma once

#include "../GLee.h"
#include "../Base/ModelManager.h"
#include "../Base/LodManager.h"
#include <string>
#include <vector>

#include "Object.h"

namespace Planet2
{
	class GroupObjectRenderableBase : public GroupObject
	{
	public:
	public:
		GroupObjectRenderableBase(World *world);
		//void draw();
		void update(C3DObject *pCamera, float dt);
		void beginDraw();
		void endDraw();
	public:
		ModelTextureGroup * getGroups();
		int getGroupsCount();
		void beginDrawGroup(ModelTextureGroup *group);
		void endDrawGroup(ModelTextureGroup *group);
		void draw(ModelTextureGroup *group);
	private:
		World *_world;
		LodManager _lods;
		Model* _model;
		CMatrix _m;
		vector<ModelTextureGroup> _groups;
		void addGroup(GLuint vbo, GLuint texture);
	protected:
		void addModel(Model* item, float minDistance, float maxDistance);
	};
}

