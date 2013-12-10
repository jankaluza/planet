#include "GroupObjectManager.h"

using namespace Planet2;

GroupObjectManager::GroupObjectManager(World *world) : _world(world)
{
}

GroupObjectManager::~GroupObjectManager(void)
{
}

void GroupObjectManager::AddObject(GroupObject *obj)
{
	GroupObjectList.push_back(obj);
}

void GroupObjectManager::draw()
{
	if (GroupObjectList.empty())
		return;

	GroupObject *main = GroupObjectList[0];

	int groupsCount = main->getGroupsCount();
	ModelTextureGroup* groups = main->getGroups();

	for (int i = 0; i < groupsCount; i++)
	{
		ModelTextureGroup *group = &groups[i];
		main->beginDrawGroup(group);

		for (vector<GroupObject *>::iterator it = GroupObjectList.begin(); it != GroupObjectList.end(); ++it) 
		{
			if ((*it)->visible)
				(*it)->draw(group);
		}

		main->endDrawGroup(group);
	}
}

void GroupObjectManager::update(C3DObject *camera, float dt)
{
	for (vector<GroupObject *>::iterator it = GroupObjectList.begin(); it != GroupObjectList.end(); ++it) 
	{
		(*it)->update(camera, dt);
	}
}

void GroupObjectManager::beginDraw()
{

}

void GroupObjectManager::endDraw()
{

}
