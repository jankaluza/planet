#include "GroupObjectRenderableBase.h"
#include "../Base/World.h"
#include "Planet.h"

using namespace Planet2;

GroupObjectRenderableBase::GroupObjectRenderableBase(World *world) : _world(world)
{
}

void GroupObjectRenderableBase::addModel(Model* item, float minDistance, float maxDistance)
{
	_lods.AddModel(item, minDistance, maxDistance);

	if (item == NULL)
		return;

	for (vector<Primitive>::iterator it = item->Primitives.begin(); it != item->Primitives.end(); ++it) 
	{
		addGroup(it->vbo, it->material->glTexDiffuse);
	}
}

void GroupObjectRenderableBase::update(C3DObject *pCamera, float dt)
{
	float dist = DistanceTo(*pCamera);
	visible = dist * dist < _world->Planet->AproximateHorizont;

	if (visible)
	{
		GetScaledModelMatrix(_m, pCamera);
		_model = _lods.GetModelAt(dist);	
	}
}

void GroupObjectRenderableBase::beginDraw()
{
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	GLfloat light_ambient[] = { 0.6, 0.6, 0.6, 1.0};
	GLfloat light_diffuse[] = { 0.0, 0.0, 0.0, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glEnable(GL_COLOR_MATERIAL);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
}

void GroupObjectRenderableBase::endDraw()
{
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_COLOR_MATERIAL);

	glPopAttrib();
}

ModelTextureGroup * GroupObjectRenderableBase::getGroups()
{
	return &_groups[0];
}

int GroupObjectRenderableBase::getGroupsCount()
{
	return _groups.size();
}

void GroupObjectRenderableBase::beginDrawGroup(ModelTextureGroup *group)
{
	if (&_groups.front() == group)
	{
		beginDraw();
	}

	glBindBuffer(GL_ARRAY_BUFFER, group->VBO);

	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, tex));
	glNormalPointer(GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, normal));
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, location)); //Vertex pointer az nakonec

	glBindTexture(GL_TEXTURE_2D, group->Texture);
}

void GroupObjectRenderableBase::endDrawGroup(ModelTextureGroup *group)
{	
	if (&_groups.back() == group)
	{
		endDraw();
	}
}

void GroupObjectRenderableBase::draw(ModelTextureGroup *group)
{
	if (_model == NULL || _model->Primitives[0].vbo != group->VBO)
		return;

	glPushMatrix();

	glMultMatrixf(_m);
	glEnable(GL_NORMALIZE);
	glScaled(m_vScale[0], m_vScale[1], m_vScale[2]);

	//Vsechna primitiva v modelu
	for (vector<Primitive>::iterator it = _model->Primitives.begin(); it != _model->Primitives.end(); ++it) 
	{
		//Pokud je to spravna skupina, vykreslime
		if (it->material->glTexDiffuse == group->Texture)
		{
			glDrawArrays(GL_TRIANGLES, it->offset, it->numVertices);
		}
	}
	glScaled(1.0f, 1.0f, 1.0f);
	glDisable(GL_NORMALIZE);

	glPopMatrix();
}

void GroupObjectRenderableBase::addGroup(GLuint vbo, GLuint texture)
{
	//Kontrola, zda jiz skupina existuje
	for (vector<ModelTextureGroup>::iterator it = _groups.begin(); it != _groups.end(); ++it) 
	{
		if (it->VBO == vbo && it->Texture == texture)
			return;
	}

	_groups.push_back(ModelTextureGroup(vbo, texture));
}
