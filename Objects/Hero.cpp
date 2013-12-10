#include "Hero.h"
#include "../Base/World.h"

using namespace Planet2;

Hero::Hero(World *world) : _tail_li(-.5f, .5f, 3, 0), _feet_li(-.6f, .6f, 4, 0), _life(3)
{
	SetScale(CVector(70));
	_torso = world->Models->GetModel("Cat_Torso.obj");
	_tail = world->Models->GetModel("Cat_Tail.obj");
	_leftFront = world->Models->GetModel("Cat_LeftFront.obj");
	_rightFront = world->Models->GetModel("Cat_RightFront.obj");
	_leftRear = world->Models->GetModel("Cat_LeftRear.obj");
	_rightRear = world->Models->GetModel("Cat_RightRear.obj");
	reset();
}

void Hero::update(C3DObject *pCamera, float dt)
{
	_camera = pCamera;
	if (Moving)
		_feet_li.Update(dt);

	_tail_li.Update(dt);
}

void Hero::beginDraw()
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

void Hero::endDraw()
{
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_COLOR_MATERIAL);

	glPopAttrib();
}

void Hero::GetMatrix(CMatrix &m, CVector translate, CVector axis, float angle)
{
	CVector vPos = m_vPosition - _camera->GetPosition();

	CQuaternion qo = *this;

	CMatrix parentRotation(qo);
	CMatrix parentTranslation;
	parentTranslation.TranslateMatrix(vPos);
	CMatrix childTranslation;
	childTranslation.TranslateMatrix(translate * m_vScale);
	CMatrix childRotation(CQuaternion(axis, angle));

	m = parentTranslation * parentRotation * childTranslation * childRotation;
	// 	GetScaledModelMatrix(m, _camera);
	return;	
}

void Hero::drawModel(Model *model, CVector translate, CVector axis, float angle)
{
	if (model == NULL)
		return;

	glPushMatrix();
	// 	std::cout << "kreslim kocku\n";

	CMatrix m;
	GetMatrix(m, translate, axis, angle);
	glMultMatrixf(m);

	glEnable(GL_NORMALIZE);
	glScaled(m_vScale[0], m_vScale[1], m_vScale[2]);

	//Vsechna primitiva v modelu
	for (vector<Primitive>::iterator it = model->Primitives.begin(); it != model->Primitives.end(); ++it) 
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

void Hero::draw()
{
	drawModel(_torso, CVector(.0f), CVector(.0f), 0);
	drawModel(_tail, CVector(-0.05f, 0.72f, 0.4f), CVector(0, 0, 1), _tail_li.Get());
	drawModel(_leftFront, CVector(-0.16f, 0.4f, -0.35f), CVector(1, 0, 0), _feet_li.Get());
	drawModel(_rightFront, CVector(0.03f, 0.43f, -0.35f), CVector(1, 0, 0), -_feet_li.Get());
	drawModel(_leftRear,  CVector(-0.14f, 0.42f, 0.32f), CVector(1, 0, 0), -_feet_li.Get());
	drawModel(_rightRear,  CVector(0.05f, 0.44f, 0.32f), CVector(1, 0, 0), _feet_li.Get());
}
