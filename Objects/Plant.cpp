#include "Plant.h"
#include "../Base/World.h"

using namespace Planet2;

Plant::Plant(World *world) : GroupObjectRenderableBase(world)
{
	//Pridame LOD - lze v libovolnem poradi
	addModel(world->Models->GetModel("Palm.obj"), 0, 200); //tento mozna nepouzijeme
	addModel(world->Models->GetModel("Palm_lod_1000.obj"), 200, 400);
	addModel(world->Models->GetModel("Palm_lod_500.obj"), 400, 1000);
	addModel(world->Models->GetModel("Palm_lod_100.obj"), 1000, 2000);
	addModel(world->Models->GetModel("Palm_lod_10.obj"), 2000, 6000);
	addModel(NULL, 6000, FLT_MAX);	
}

