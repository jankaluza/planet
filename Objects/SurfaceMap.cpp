// SurfaceMap.cpp
//

//#include "Master.h"
#include "SurfaceMap.h"

void CSurfaceMap::Init(int nResolution, CHeightMap *pHeightMap, CColor *pPalette, int nColors)
{
	CPlanetaryMap::Init(nResolution, sizeof(CColor));
	m_pHeightMap = pHeightMap;
	m_pPalette = pPalette;
	m_nColors = nColors;

	m_pbRaw.Init(m_nColors, 1, 4, GL_RGBA, GL_UNSIGNED_BYTE, m_pPalette);
	m_tRaw.Init(&m_pbRaw);

	if(nResolution)
	{
		float *pHeight = (float *)pHeightMap->GetBuffer();
		CColor *pColor = (CColor *)GetBuffer();
		for(int nFace=0; nFace<6; nFace++)
		{
			for(int y=0; y<m_nResolution; y++)
			{
				for(int x=0; x<m_nResolution; x++)
				{
					float fHeight = *pHeight - m_pHeightMap->GetRadius() + m_pHeightMap->GetMaxHeight();
					fHeight = (m_nColors-1) * Clamp(0.001f, 0.999f, fHeight / (2.0f * m_pHeightMap->GetMaxHeight()));
					int nIndex = (int)fHeight;
					float fRatio = fHeight - nIndex;
					*pColor = pPalette[nIndex] * (1-fRatio) + pPalette[nIndex+1] * fRatio;

					pColor++;
					pHeight++;
				}
			}
			m_pbMap[nFace].Init(m_nResolution, m_nResolution, 4, GL_RGBA, GL_UNSIGNED_BYTE, GetFaceBuffer(nFace));
			m_tMap[nFace].Init(&m_pbMap[nFace]);
		}
	}
}
