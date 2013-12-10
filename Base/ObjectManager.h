#pragma once

#include "../Objects/Object.h"
#include <vector>

using namespace std;

class C3DObject;

namespace Planet2
{
	class World;

	typedef vector<Object *> ObjectList;
	typedef vector<ObjectList> OrderList;

	class ObjectManager : public Object
	{
	public:
		ObjectManager(World *world, int order_levels);
		virtual ~ObjectManager();
		virtual void AddObject(Object *obj, int order);
		virtual void RemoveObject(Object *obj, int order);
		virtual void beginDraw();
		virtual void endDraw();
		void draw();
		void update(C3DObject *camera, float dt);

		ObjectList &getSolidObjects() {
			return _solidObjects;
		}
	protected:
		World *_world;
		int _order_levels;
		OrderList _objects;
		ObjectList _solidObjects;
	};
}

