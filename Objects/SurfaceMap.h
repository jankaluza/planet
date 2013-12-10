// SurfaceMap.h
//

#ifndef __SurfaceMap_h__
#define __SurfaceMap_h__

#include "PlanetaryMap.h"
#include "../Base/PixelBuffer.h"
#include "../Base/Texture.h"
#include "../Base/Matrix.h"
#include "HeightMap.h"

class CColor;

class CSurfaceMap : public CPlanetaryMap
{
protected:
	CHeightMap *m_pHeightMap;
	CColor *m_pPalette;
	int m_nColors;

	CPixelBuffer m_pbRaw;
	CTexture m_tRaw;
	CPixelBuffer m_pbMap[6];
	CTexture m_tMap[6];

public:
	CSurfaceMap() : CPlanetaryMap() {}

	CHeightMap *GetHeightMap()					{ return m_pHeightMap; }
	CTexture &GetTextureMap(int nFace)			{ return m_tMap[nFace]; }
	CTexture &GetRawTextureMap()				{ return m_tRaw; }

	void GetTextureCoordinates(const CVector &v, int nFace, float &x, float &y)
	{
		GetCoordinates(v, nFace, x, y);
	}
	void GetRawTextureCoordinates(const CVector &v, float fHeight, float &x, float &y)
	{
		x = fHeight - m_pHeightMap->GetRadius() + m_pHeightMap->GetMaxHeight();
		x = Clamp(0.001f, 0.999f, x / (2.0f * m_pHeightMap->GetMaxHeight()));
		y = 0;
	}

	void Init(int nResolution, CHeightMap *pHeightMap, CColor *pPalette, int nColors);
};

#endif // __SurfaceMap_h__
