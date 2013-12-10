#include "ObjectManager.h"

#include "../Objects/Object.h"
#include "Matrix.h"

#include <vector>
#include <algorithm>

using namespace Planet2;
using namespace std;

ObjectManager::ObjectManager(World *world, int order_levels) : _world(world), _order_levels(order_levels) {
	_objects.resize(_order_levels);
}

ObjectManager::~ObjectManager() {
	// 	for (vector<Object *>::const_iterator it = _objects.begin(); it != _objects.end(); ++it) {
	// 		delete (*it);
	// 	}
}

void ObjectManager::AddObject(Object *obj, int order) {
	_objects[order].push_back(obj);

	if (obj->solid) {
		_solidObjects.push_back(obj);
	}

	obj->managed = true;
}

void ObjectManager::RemoveObject(Object *obj, int order) {
	obj->managed = false;
	ObjectList &l = _objects[order];
	ObjectList::iterator it = std::find(l.begin(), l.end(), obj);
	if (it != l.end()) {
		l.erase(it);
	}

	if (obj->solid) {
		ObjectList::iterator is = std::find(_solidObjects.begin(), _solidObjects.end(), obj);
		if (is != _solidObjects.end()) {
			_solidObjects.erase(is);
		}
	}
}

void ObjectManager::beginDraw()
{}

void ObjectManager::endDraw()
{}

void ObjectManager::draw() {
	for (OrderList::const_iterator it = _objects.begin(); it != _objects.end(); ++it) {
		for (ObjectList::const_iterator obj = it->begin(); obj != it->end(); ++obj) {
			if ((*obj)->visible)
			{
				(*obj)->beginDraw();
				(*obj)->draw();
				(*obj)->endDraw();
			}
		}
	}
}


void ObjectManager::update(C3DObject *camera, float dt) 
{
	for (OrderList::const_iterator it = _objects.begin(); it != _objects.end(); ++it) {
		for (ObjectList::const_iterator obj = it->begin(); obj != it->end(); ++obj) {
			(*obj)->update(camera, dt);
		}
	}
}