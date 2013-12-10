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

#ifndef __Noise_h__
#define __Noise_h__

#include "Math.h"
#include <math.h>
#include <stdlib.h>

// Defines
#define PI					3.14159f			// PI
#define HALF_PI				1.57080f			// PI / 2
#define TWO_PI				6.28318f			// PI * 2
#define INV_PI				0.318310f			// 1 / PI
#define INV_TWO_PI			0.159155f			// 1 / (PI*2)
#define INV_HALF_PI			0.636618f			// 1 / (PI/2)

#define LOGHALF				-0.693147f			// log(0.5)
#define LOGHALFI			-1.442695f			// Inverse of log(0.5)
#define DELTA				1e-6f				// Small number for comparing floating point numbers
#define MAX_DIMENSIONS		4					// Maximum number of dimensions in a noise object
#define MAX_OCTAVES			128					// Maximum # of octaves in an fBm object

#define HALF_RAND			(RAND_MAX/2)

// Macros
#define SQUARE(a)			((a) * (a))
#define FLOOR(a)			((int)(a) - ((a) < 0 && (a) != (int)(a)))
#define CEILING(a)			((int)(a) + ((a) > 0 && (a) != (int)(a)))
#define MIN(a, b)			((a) < (b) ? (a) : (b))
#define MAX(a, b)			((a) > (b) ? (a) : (b))
#define ABS(a)				((a) < 0 ? -(a) : (a))
#define CLAMP(a, b, x)		((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))
#define LERP(a, b, x)		((a) + (x) * ((b) - (a)))
#define CUBIC(a)			((a) * (a) * (3 - 2*(a)))
#define STEP(a, x)			((x) >= (a))
#define BOXSTEP(a, b, x)	Clamp(0, 1, ((x)-(a))/((b)-(a)))
#define PULSE(a, b, x)		(((x) >= (a)) - ((x) >= (b)))
#define GAMMA(a, g)			CMath::pow(a, 1/g)
#define BIAS(a, b)			CMath::pow(a, logf(b) * LOGHALFI)
#define EXPOSE(l, k)		(1 - CMath::exp(l * k))
#define DEGTORAD(x)			((x) * 0.01745329251994f)
#define RADTODEG(x)			((x) * 57.29577951308f)
#define SWAP(a, b, t)		{ t = a; a = b; b = t; }

// Inline functions (use instead of macros to avoid performing slow operations twice)
inline float Square(float a)					{ return a * a; }
inline int Floor(float a)						{ return ((int)a - (a < 0 && a != (int)a)); }
inline int Ceiling(float a)						{ return ((int)a + (a > 0 && a != (int)a)); }
inline float Min(float a, float b)				{ return (a < b ? a : b); }
inline float Max(float a, float b)				{ return (a > b ? a : b); }
inline float Abs(float a)						{ return (a < 0 ? -a : a); }
inline float Clamp(float a, float b, float x)	{ return (x < a ? a : (x > b ? b : x)); }
inline float Lerp(float a, float b, float x)	{ return a + x * (b - a); }
inline float Cubic(float a)						{ return a * a * (3 - 2*a); }
inline float Step(float a, float x)				{ return (float)(x >= a); }
inline float Boxstep(float a, float b, float x)	{ return Clamp(0, 1, (x-a)/(b-a)); }
inline float Pulse(float a, float b, float x)	{ return (float)((x >= a) - (x >= b)); }
inline float Gamma(float a, float g)			{ return CMath::pow(a, 1/g); }
inline float Bias(float a, float b)				{ return CMath::pow(a, logf(b) * LOGHALFI); }

inline float Gain(float a, float b)
{
	if(a <= DELTA)
		return 0;
	if(a >= 1-DELTA)
		return 1;

	register float p = (logf(1 - b) * LOGHALFI);
	if(a < 0.5)
		return CMath::pow(2 * a, p) * 0.5f;
	else
		return 1 - CMath::pow(2 * (1 - a), p) * 0.5f;
}

inline float Smoothstep(float a, float b, float x)
{
	if(x <= a)
		return 0;
	if(x >= b)
		return 1;
	return Cubic((x - a) / (b - a));
}

inline float Mod(float a, float b)
{
	a -= ((int)(a / b)) * b;
	if(a < 0)
		a += b;
	return a;
}

inline void Normalize(float *f, int n)
{
	float fMagnitude = 0;
	int i;
	for(i=0; i<n; i++)
		fMagnitude += f[i]*f[i];
	fMagnitude = 1 / CMath::fastsqrt(fMagnitude);
	for(i=0; i<n; i++)
		f[i] *= fMagnitude;
}

/*******************************************************************************
* Class: CRandom
********************************************************************************
* This class wraps a random number generator. I plan to implement my own random
* number generator so I can keep the seeds as member variables (which is more
* flexible than using statics or globals). I was using one I found on the
* Internet implemented in assembler, but I was having problems with it so I
* removed it for this demo.
*******************************************************************************/
class CRandom
{
public:
	CRandom()						{}
	CRandom(unsigned int nSeed)		{ Init(nSeed); }
	void Init(unsigned int nSeed)	{ srand(nSeed); }
	double Random()					{ return (double)rand()/(double)RAND_MAX; }
	double RandomD(double dMin, double dMax)
	{
		double dInterval = dMax - dMin;
		double d = dInterval * Random();
		return dMin + MIN(d, dInterval);
	}
	unsigned int RandomI(unsigned int nMin, unsigned int nMax)
	{
		unsigned int nInterval = nMax - nMin;
		unsigned int i = (unsigned int)((nInterval+1.0) * Random());
		return nMin + MIN(i, nInterval);
	}
};

/*******************************************************************************
* Class: CNoise
********************************************************************************
* This class implements the Perlin noise function. Initialize it with the number
* of dimensions (1 to 4) and a random seed. I got the source for the first 3
* dimensions from "Texturing & Modeling: A Procedural Approach". I added the
* extra dimension because it may be desirable to use 3 spatial dimensions and
* one time dimension. The noise buffers are set up as member variables so that
* there may be several instances of this class in use at the same time, each
* initialized with different parameters.
*******************************************************************************/
class CNoise
{
protected:
	int m_nDimensions;						// Number of dimensions used by this object
	unsigned char m_nMap[256];				// Randomized map of indexes into buffer
	float m_nBuffer[256][MAX_DIMENSIONS];	// Random n-dimensional buffer

	float Lattice(int ix, float fx, int iy=0, float fy=0, int iz=0, float fz=0, int iw=0, float fw=0)
	{
		int n[4] = {ix, iy, iz, iw};
		float f[4] = {fx, fy, fz, fw};
		int nIndex = 0;
		int i;
		for(i=0; i<m_nDimensions; i++)
			nIndex = m_nMap[(nIndex + n[i]) & 0xFF];
		float fValue = 0;
		for(i=0; i<m_nDimensions; i++)
			fValue += m_nBuffer[nIndex][i] * f[i];
		return fValue;
	}

public:
	CNoise()	{}
	CNoise(int nDimensions, unsigned int nSeed)	{ Init(nDimensions, nSeed); }
	void Init(int nDimensions, unsigned int nSeed);
	float Noise(float *f);
};

/*******************************************************************************
* Class: CSeededNoise
********************************************************************************
*******************************************************************************/
class CSeededNoise
{
protected:
	float m_nBuffer[64][64];

	float Lattice(int ix, float fx, int iy=0, float fy=0, int iz=0, float fz=0)
	{
		float fValue = m_nBuffer[ix][iy];
		return fValue;
	}

public:
	CSeededNoise()	{}
	CSeededNoise(unsigned int nSeed)	{ Init(nSeed); }
	void Init(unsigned int nSeed);
	float Noise(float *f);
};

/*******************************************************************************
* Class: CFractal
********************************************************************************
* This class implements fBm, or fractal Brownian motion. Since fBm uses Perlin
* noise, this class is derived from CNoise. Initialize it with the number of
* dimensions (1 to 4), a random seed, H (roughness ranging from 0 to 1), and
* the lacunarity (2.0 is often used). Many of the fractal routines came from
* "Texturing & Modeling: A Procedural Approach". fBmTest() is my own creation,
* and I created it to generate my first planet.
*******************************************************************************/
class CFractal : public CNoise
{
protected:
	float m_fH;
	float m_fLacunarity;
	float m_fExponent[MAX_OCTAVES];

public:
	CFractal()	{}
	CFractal(int nDimensions, unsigned int nSeed, float fH, float fLacunarity)
	{
		Init(nDimensions, nSeed, fH, fLacunarity);
	}
	void Init(int nDimensions, unsigned int nSeed, float fH, float fLacunarity)
	{
		CNoise::Init(nDimensions, nSeed);
		m_fH = fH;
		m_fLacunarity = fLacunarity;
		float f = 1;
		for(int i=0; i<MAX_OCTAVES; i++) 
		{
			m_fExponent[i] = CMath::pow(f, -m_fH);
			f *= m_fLacunarity;
		}
	}
	float fBm(float *f, float fOctaves);
	float Turbulence(float *f, float fOctaves);
	float Multifractal(float *f, float fOctaves, float fOffset);
	float Heterofractal(float *f, float fOctaves, float fOffset);
	float HybridMultifractal(float *f, float fOctaves, float fOffset, float fGain);
	float RidgedMultifractal(float *f, float fOctaves, float fOffset, float fThreshold);
	float fBmTest(float *f, int nStart, int nEnd, float fInitial=0.0f);
	float fBmTest(float *f, float fOctaves);
};

#endif // __Noise_h__
