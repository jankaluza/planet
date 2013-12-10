#pragma once

#include "../Objects/Object.h"
#include <vector>

using namespace std;
class C3DObject;

namespace Planet2
{
	class GroupObject;
	class World;

	class GroupObjectManager : public Object
	{

	public:
		GroupObjectManager(World *world);
		virtual ~GroupObjectManager(void);

		virtual void AddObject(GroupObject *obj);
		void draw();
		void update(C3DObject *camera, float dt);
		void beginDraw();
		void endDraw();
		vector<GroupObject *> GroupObjectList;
	protected:		
		World *_world;
	};
}
