#include "Box.h"
#include "../Base/World.h"

using namespace Planet2;

Box::Box(World *world) : GroupObjectRenderableBase(world)
{
	SetScale(CVector(70));
	addModel(world->Models->GetModel("WoodenBox.obj"), 0, 1000);
	addModel(world->Models->GetModel("WoodenBox_lod_300.obj"), 1000, 1500);
	addModel(world->Models->GetModel("WoodenBox_lod_12.obj"), 1500, 3000);
	addModel(NULL, 3000, FLT_MAX);	
}