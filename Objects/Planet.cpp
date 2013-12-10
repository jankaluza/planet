// Planet.cpp
//

//#include "Master.h"
#include "Planet.h"
#include <iostream>
#include <algorithm>

using namespace Planet2;

void CPlanet::Init()
{
	Cleanup();

	// Load some values into local variables for initializing the maps
	int nSeed = 1046527;
	int nResolution = 0; //nejlepe nechat 0
	float fRadius = 1378;
	float fMaxHeight = 150;
	float fWaterLevel = 0;
	float fOctaves = 3;
	float fRoughness = 0.5f;

	m_nColors = 16;
	m_pColors = new CColor[m_nColors];
	m_pColors[0]=CColor(48, 49, 44, 255);
	m_pColors[1]=CColor(73, 59, 50, 255);
	m_pColors[2]=CColor(90, 63, 54, 255);
	m_pColors[3]=CColor(91, 66, 59, 255);
	m_pColors[4]=CColor(101, 66, 47, 255);
	m_pColors[5]=CColor(104, 68, 54, 255);
	m_pColors[6]=CColor(112, 69, 53, 255);
	m_pColors[7]=CColor(137, 75, 50, 255);
	m_pColors[8]=CColor(127, 75, 51, 255);
	m_pColors[9]=CColor(161, 87, 48, 255);
	m_pColors[10]=CColor(185, 101, 57, 255);
	m_pColors[11]=CColor(172, 95, 53, 255);
	m_pColors[12]=CColor(216, 123, 66, 255);
	m_pColors[13]=CColor(239, 144, 57, 255);
	m_pColors[14]=CColor(202, 112, 60, 255);
	m_pColors[15]=CColor(255, 255, 255, 255);	

	// Initialize the planetary maps and ROAM spheres
	m_mapHeight.InitFractal(nSeed, fOctaves, fRoughness);
	m_mapHeight.Init(nResolution, fRadius, fMaxHeight, fWaterLevel);
	m_mapSurface.Init(nResolution, &m_mapHeight, m_pColors, m_nColors);
	CROAMSphere::Init(&m_mapHeight, &m_mapSurface);
}

void CPlanet::Cleanup()
{
	if(m_pColors)
	{
		delete[] m_pColors;
		m_pColors = NULL;
	}
}

void CPlanet::AddObject(Object *obj, float lat, float lon) 
{
	obj->SetPosition(CVectorFromSpherical(lat, lon));
	AddObject(obj);
}

void CPlanet::AddObject(Object *obj, int order)
{
	RefreshObject(obj);
	ObjectManager::AddObject(obj, order);
}

void CPlanet::RefreshObject(Object *obj)
{
	CVector pos = obj->GetPosition();

	// Get the height of the place specified by vector to compute ideal
	// position of the object
	float fHeight = m_mapHeight.GetHeight(pos, pos.Magnitude());
	pos.Normalize();
	pos *= fHeight;

	// Associate object with particular triangle on the planet, so it can change its
	// position later when the planet is updated (see onTriangleUpdated)
	CROAMTriangle *tri = getCords(pos * -1, pos);
	if (tri) {
		if (obj->tri != tri) {
			if (obj->tri) {
				std::vector<Object *>::iterator it = std::find(obj->tri->objects.begin(), obj->tri->objects.end(), obj);
				if (it != obj->tri->objects.end()) {
					obj->tri->objects.erase(it);
				}
			}
			obj->tri = tri;
			tri->objects.push_back(obj);
		}

		obj->SetPosition(pos);

		if (obj->rotateWithTriangle) {
			obj->RotateToVector(tri->GetNormal());
		}
		else {
			obj->RotateToVector(pos, true);
		}
	}
	else {
		obj->RotateToVector(pos);
		std::cerr << "WARNING: No triangle for newly added object?!\n";
	}
}

void CPlanet::AddObjectGroup(GroupObjectManager *obj) 
{	
	for (vector<GroupObject *>::iterator it = obj->GroupObjectList.begin(); it != obj->GroupObjectList.end(); ++it) 
	{
		AddObject(*it);
	}

	ObjectManager::AddObject(obj, 0);
}

void CPlanet::draw()
{
	//return;
	bool bTexture = true;

	// Multiply the current view matrix by the model matrix and draw the planet
	glPushMatrix();
	glMultMatrixf(_m);

	glEnable(GL_NORMALIZE);		// We have to enable this because we're using a scaled MODELVIEW matrix

	float fBlendRatio = (m_fAltitude / m_mapHeight.GetRadius() - 0.25f);
	bool bFaceTextures = false;

	if(bTexture)
	{
		bFaceTextures = m_mapSurface.GetResolution() != 0 && (fBlendRatio > 0.0f);
		if(bFaceTextures)
		{
			// We're far enough away that it's better to use the pre-computed face textures
			GLUtil()->EnableTextureCoordArray(2);
		}
		else
		{
			// We're close enough to use the raw texture and detailed noise texture
			GLUtil()->EnableTextureCoordArray();
			m_mapSurface.GetRawTextureMap().Enable();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
	}

	BuildIndexList();
	for(int nFace=0; nFace<6; nFace++)
	{
		if(bFaceTextures)
			m_mapSurface.GetTextureMap(nFace).Enable();
		DrawFace(nFace);
		if(bFaceTextures)
			m_mapSurface.GetTextureMap(nFace).Disable();
	}

	if(bTexture)
	{
		GLUtil()->DisableTextureCoordArray();
		CTexture::Disable(GL_TEXTURE_1D);	// Needed for now because raw texture is 1D. This should change later.
		CTexture::Disable(GL_TEXTURE_2D);
	}

	glDisable(GL_NORMALIZE);	// We have to enable this because we're using a scaled MODELVIEW matrix

	glPopMatrix();
}

void CPlanet::beginDraw()
{
	int nPolygonMode = GL_FILL; //GL_FILL;// GL_LINE;
	bool bTexture = true;

	GLUtil()->InitRenderContext();

	glPolygonMode(GL_FRONT, nPolygonMode);
}

void CPlanet::endDraw()
{
	glPolygonMode(GL_FRONT, GL_FILL);

	GLUtil()->StopRenderContext();

	ObjectManager::draw();
}

void CPlanet::onTriangleUpdated(CROAMTriangle *tri) {
	CVector pos(0, 0, 0);

	// Iterate over all objects associated with this triangle, compute their
	// intersection point with the triangle and update the position
	for (std::vector<Object *>::const_iterator it = tri->objects.begin(); it != tri->objects.end(); ++it) {
		Object *obj = *it;
		CVector v = obj->GetPosition() * -1;
		if (tri->CheckIntersect(v, pos)) {
			obj->SetPosition(pos);
			if (obj->rotateWithTriangle) {
				obj->RotateToVector(tri->GetNormal());
			}
		}
	}
}


