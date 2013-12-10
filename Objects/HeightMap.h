// HeightMap.h
//
#ifndef __HeightMap_h__
#define __HeightMap_h__

#include "PlanetaryMap.h"


class CHeightMap : public CPlanetaryMap
{
protected:
	// Members for fractal routine
	CFractal m_frHeight;
	int m_nFractalSeed;
	float m_fOctaves;
	float m_fRoughness;

	// Members for planet parameters
	float m_fRadius;
	float m_fMaxHeight;
	float m_fWaterLevel;

public:
	CHeightMap() : CPlanetaryMap()
	{
		m_nFractalSeed = 0;
	}

	float GetRadius()		{ return m_fRadius; }
	float GetMaxHeight()	{ return m_fMaxHeight; }
	float GetWaterLevel()	{ return m_fWaterLevel; }

	void InitFractal(int nSeed, float fOctaves, float fRoughness)
	{
		m_nFractalSeed = nSeed;
		m_fOctaves = fOctaves;
		m_fRoughness = fRoughness;
		m_frHeight.Init(3, m_nFractalSeed, 1.0f - m_fRoughness, 2.0f);
	}
	void Init(int nResolution, float fRadius, float fMaxHeight=0, float fWaterLevel=-1.0E10f);

	float GetHeightNormalized(const CVector &vNormalized);
	float GetHeight(const CVector &vPosition, float fHeight)		{ return GetHeightNormalized(vPosition / fHeight); }
	float GetHeightOffset(const CVector &vPosition, float fHeight)	{ return GetHeightNormalized(vPosition / fHeight) - fHeight; }
	float GetHeight(const CVector &vPosition)						{ return GetHeight(vPosition, vPosition.Magnitude()); }
	float GetHeightOffset(const CVector &vPosition)					{ return GetHeightOffset(vPosition, vPosition.Magnitude()); }
};

#endif // __HeightMap_h__
