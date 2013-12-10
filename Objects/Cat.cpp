#include "Cat.h"

#include "../Base/World.h"
#include "Planet.h"

using namespace Planet2;

Cat::Cat(World *world) : _world(world)
{
	SetScale(CVector(100));
	_model = _world->Models->GetModel("Cat2.obj");	
}

void Cat::beginDraw()
{
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
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

void Cat::endDraw()
{
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_COLOR_MATERIAL);

	glPopAttrib();
}

void Cat::update(C3DObject *camera, float dt)
{
	float dist = DistanceTo(*camera);
	visible = dist * dist < _world->Planet->AproximateHorizont;

	if (visible)
	{
		GetScaledModelMatrix(_m, camera);	
	}
}

void Cat::draw()
{
	if (_model == NULL)
		return;

	glPushMatrix();

	glMultMatrixf(_m);

	glEnable(GL_NORMALIZE);
	glScaled(m_vScale[0], m_vScale[1], m_vScale[2]);

	//Vsechna primitiva v modelu
	for (vector<Primitive>::iterator it = _model->Primitives.begin(); it != _model->Primitives.end(); ++it) 
	{
		glBindBuffer(GL_ARRAY_BUFFER, it->vbo);

		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, tex));
		glNormalPointer(GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, normal));
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid *) offsetof(Vertex, location)); //Vertex pointer az nakonec

		glBindTexture(GL_TEXTURE_2D, it->material->glTexDiffuse);
		glDrawArrays(GL_TRIANGLES, it->offset, it->numVertices);
	}

	glScaled(1.0f, 1.0f, 1.0f);
	glDisable(GL_NORMALIZE);

	glPopMatrix();
}

