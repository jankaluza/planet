#pragma once
#include "GroupObjectRenderableBase.h"

namespace Planet2
{
	class World;	

	class Box : public GroupObjectRenderableBase
	{
	public:
		Box(World *world);
	};
}

