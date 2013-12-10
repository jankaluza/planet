
#pragma once

#include "ROAMSphere.h"
#include "../Base/Matrix.h"
#include "../Base/GroupObjectManager.h"
#include "../Base/ObjectManager.h"
#include "HeightMap.h"
#include "SurfaceMap.h"
#include "Object.h"

namespace Planet2 {

	class World;

	class CPlanet : public ObjectManager, public CROAMSphere
	{
	public:
		float AproximateHorizont;			// The aproximate distance from camera to the planet's horizont (squared)
	public:
		CHeightMap m_mapHeight;
		CSurfaceMap m_mapSurface;

		CVector m_vCameraPosition;			// The position of the camera (relative to the planet)
		CQuaternion m_qCameraOrientation;	// The orientation of the camera (relative to the planet)
		float m_fHeight;					// The distance between the camera and the ground directly beneath it
		float m_fAltitude;					// The distance between the camera and the planet (using average radius)		
		float m_fHorizon;					// The distance from the camera to the planet's horizon (squared)
		int m_nColors;
		CColor *m_pColors;
		CMatrix _m;		

	public:
		CPlanet(World *world) : ObjectManager(world, 2)
		{		
			m_pColors = NULL;
		}
		virtual ~CPlanet()
		{
			Cleanup();
		}
		void Init();
		void Cleanup();

		virtual void AddObjectGroup(GroupObjectManager *obj);
		void AddObject(Object *obj, float lat, float lon);		
		void AddObject(Object *obj, int order = 0);
		void RefreshObject(Object *obj);

		float GetHeight()		{ return m_fHeight; }
		float GetAltitude()		{ return m_fAltitude; }
		float GetHorizon()		{ return m_fHorizon; }

		float GetHeight(CVector position) {
			return m_mapHeight.GetHeight(position);
		}

		void SetViewpoint(C3DObject *pCamera)
		{
			//Rotate(CVector(0, 1, 0), m_fAngularVelocity*fSeconds);
			m_vCameraPosition = UnitInverse().RotateVector(pCamera->GetPosition() - GetPosition());
			m_qCameraOrientation = *pCamera;
			m_qCameraOrientation.Rotate(UnitInverse());

			float fOriginalHeight = m_vCameraPosition.Magnitude();
			m_fHeight = fOriginalHeight - m_mapHeight.GetHeight(m_vCameraPosition, fOriginalHeight);
			m_fAltitude = Max(1.0f, fOriginalHeight - m_mapHeight.GetRadius());
			m_fHorizon = m_fAltitude*m_fAltitude + 2.0f*m_fAltitude*m_mapHeight.GetRadius();	// Distance to horizon squared
			
			AproximateHorizont = pCamera->GetPosition().MagnitudeSquared() + m_mapHeight.GetRadius() * m_mapHeight.GetRadius();
		}
		bool CollisionCheck()	{ return m_fHeight < DELTA; }
		bool CollisionCheck(C3DObject *pCamera)
		{
			CVector vCameraPosition = UnitInverse().RotateVector(pCamera->GetPosition() - GetPosition());
			float fOriginalHeight = vCameraPosition.Magnitude();
			m_fHeight = fOriginalHeight - m_mapHeight.GetHeight(m_vCameraPosition, fOriginalHeight);
			return m_fHeight < DELTA;
		}

		void update(C3DObject *pCamera, float dt)
		{
			SetViewpoint(pCamera);
			GetScaledModelMatrix(_m, pCamera);

			float fMaxError = 0.0080f;
			CVector vHeading = m_qCameraOrientation.GetViewAxis();		
			CROAMSphere::Update(m_vCameraPosition, vHeading, m_fHorizon, fMaxError);
			ObjectManager::update(pCamera, dt);
		}
		void draw();	
		void beginDraw();
		void endDraw();
		void onTriangleUpdated(CROAMTriangle *tri);
	};

}
