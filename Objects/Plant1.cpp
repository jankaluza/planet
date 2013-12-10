#include "Plant1.h"
#include "../Base/World.h"

using namespace Planet2;

Plant1::Plant1(World *world) : GroupObjectRenderableBase(world)
{
	addModel(world->Models->GetModel("Plant1_lod_1000.obj"), 0, 600);
	addModel(world->Models->GetModel("Plant1_lod_500.obj"), 600, 1200);
	addModel(world->Models->GetModel("Plant1_lod_100.obj"), 1200, 3000);
	addModel(NULL, 3000, FLT_MAX);	
}
