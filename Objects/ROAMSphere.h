/*
	The Universe Development Kit
	Copyright (C) 2000  Sean O'Neil
	s_p_oneil@hotmail.com

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __ROAMSphere_h__
#define __ROAMSphere_h__

//#include "SurfaceMap.h"
#include "ROAMDiamond.h"
#include "ROAMTriangle.h"
#include "HeightMap.h"
#include "SurfaceMap.h"
#include "../Base/Matrix.h"
#include "../Base/ListTemplates.h"

/****************************************************************************
* Class: CROAMSphere
*****************************************************************************
* This class implements the spherical ROAM algorithm. It keeps lists of the
* vertices, triangles, and diamonds. Because it is currently used to implement
* a fractal-based planet, it also has a member variable of the CFractal class
* and a number of others for defining the planet's basic properties. Its main
* methods are Init(), Update(), and Draw(). Init() initializes the planet
* with a random seed and certain parameters, like radius and roughness.
* Update() updates the priority lists and performs any necessary splits or
* merges in the mesh. Draw() draws the mesh using either smooth or flat shading.
****************************************************************************/
class CROAMSphere
{
protected:
	static unsigned short m_nIndex[128*1024*3];
	static int m_nStart[6];
	static int m_nEnd[6];

	CROAMTriangleList m_llTriangle[6];
	CROAMDiamondList m_llDiamond;
	CHeightMap *m_pHeightMap;
	CSurfaceMap *m_pSurfaceMap;
	int m_nTriangles;
	float m_fMaxError;

	void RebuildEdgeList();
	void BuildCube();
	void UpdateTriangleOffset(CROAMTriangle *pTriangle);
	bool Split(CROAMTriangle *pTriangle);
	void Merge(CROAMDiamond *pDiamond);

public:
	CROAMSphere()
	{
		m_fMaxError = 0.0003f;
		m_nTriangles = 0;
	}
	~CROAMSphere()
	{
	}

	int GetTriangleCount()	{ return m_nTriangles; }

	void Init(CHeightMap *pHeightMap, CSurfaceMap *pSurfaceMap=NULL);
	void Update(CVector vPosition, CVector vHeading, float fHorizon, float fMaxError);

	CROAMTriangle *getCords(CVector v, CVector &point);

	void BuildIndexList()
	{
		m_nTriangles = 0;
		CROAMTriangle *pTriangle;
		int nFace, nIndex = 0;
		for(nFace=0; nFace<6; nFace++)
		{
			m_nStart[nFace] = nIndex;
			for(pTriangle=m_llTriangle[nFace].GetHead(); pTriangle->IsInList(); pTriangle = pTriangle->GetNext())
			{
				pTriangle->Draw(m_nIndex, nIndex);
				m_nTriangles++;
			}
			m_nEnd[nFace] = nIndex;
		}
	}

	virtual void onTriangleUpdated(CROAMTriangle *tri) = 0;

	void Draw()
	{
		int nFace;
		BuildIndexList();
		for(nFace=0; nFace<6; nFace++)
			DrawFace(nFace);
	}
	void DrawFace(int nFace)
	{
		int nStart = m_nStart[nFace];
		int nSize = m_nEnd[nFace] - nStart;
		while(nSize > 60000)
		{
			glDrawElements(GL_TRIANGLES, 60000, GL_UNSIGNED_SHORT, &m_nIndex[nStart]);
			nSize -= 60000;
			nStart += 60000;
		}
		glDrawElements(GL_TRIANGLES, nSize, GL_UNSIGNED_SHORT, &m_nIndex[nStart]);
	}
};

#endif // __ROAMSphere_h__
