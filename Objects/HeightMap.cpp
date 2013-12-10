// HeightMap.cpp
//

//#include "Master.h"
#include "HeightMap.h"

void CHeightMap::Init(int nResolution, float fRadius, float fMaxHeight, float fWaterLevel)
{
	m_fRadius = fRadius;
	m_fMaxHeight = fMaxHeight;
	m_fWaterLevel = fWaterLevel;

	if(nResolution)
	{
		CPlanetaryMap::Init(nResolution, sizeof(float));
		float *pCell = (float *)GetBuffer();
		for(int nFace=0; nFace<6; nFace++)
		{
			for(int y=0; y<m_nResolution; y++)
			{
				for(int x=0; x<m_nResolution; x++)
				{
					*pCell = GetHeight(GetDirectionVector(nFace, x, y));
					pCell++;
				}
			}
		}
	}
}

float CHeightMap::GetHeightNormalized(const CVector &vNormalized)
{
	// The height always starts out as the radius
	float fHeight = m_fRadius;
	CVector v = vNormalized;

	// If a fractal seed was passed in, adjust the height based on the fractal routine
	if(m_nFractalSeed)
		fHeight += m_frHeight.fBmTest(v, m_fOctaves) * m_fMaxHeight;

	// Adjust the height by any other custom method you desire

	// Return the final adjusted height
	return fHeight;
}
