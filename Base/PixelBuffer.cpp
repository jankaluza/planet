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

//#include "Master.h"
#include "PixelBuffer.h"


void CPixelBuffer::MakeRandom(unsigned int nSeed)
{
	// Instead of calling Random() to fill a byte every pixel,
	// call it every 4 pixels to fill a dword
	srand(nSeed);
	int nSize = (m_nWidth * m_nHeight * m_nChannels) >> 1;
	unsigned short *pDest = (unsigned short *)m_pBuffer;
	for(int i=0; i<nSize; i++)
		*pDest++ = rand();
}

void CPixelBuffer::MakeNoise(unsigned long nSeed)
{
	const unsigned char nMin = 224, nMax = 255;
	const float fHalf = (nMax + 1 - nMin) / 2;

	CFractal fractal(2, nSeed, 0.5f, 2.0f);
	int nHalfWidth = m_nWidth >> 1, nHalfHeight = m_nHeight >> 1;
	float f[2];
	unsigned char *pBuffer = (unsigned char *)GetBuffer();
	for(int y=0; y<m_nHeight; y++)
	{
		for(int x=0; x<m_nWidth; x++)
		{
			if(x < nHalfWidth+1 && y < nHalfHeight+1)
			{
				f[0] = 32.0f * (float)x / (float)m_nWidth;
				f[1] = 32.0f * (float)y / (float)m_nHeight;
				float fValue = fractal.fBm(f, 4.0f);
				unsigned char nValue = (unsigned char)Max(nMin, Min(nMax, ((fValue+1.0f) * fHalf + nMin)));
				*pBuffer++ = nValue;
			}
			else
			{
				int xTemp = x, yTemp = y;
				if(xTemp > nHalfWidth)
					xTemp = m_nWidth - xTemp;
				if(yTemp > nHalfHeight)
					yTemp = m_nHeight - yTemp;
				*pBuffer++ = *(unsigned char *)operator()(xTemp, yTemp);
			}
		}
	}
}

void CPixelBuffer::MakeGlow(float fExpose, float fSizeDisc)
{
	int i;
	int n = 0;
	unsigned char nIntensity;
	for(int y=0; y<m_nHeight; y++)
	{
		float fDy = (float)y/(m_nHeight-1) - 0.5f;
		for(int x=0; x<m_nWidth; x++)
		{
			float fDx = (float)x/(m_nWidth-1) - 0.5f;
			float fDist = CMath::fastsqrt(fDx*fDx + fDy*fDy);
			float fIntensity = CMath::exp(-Max(fDist-fSizeDisc,0)*fExpose);
			switch(m_nDataType)
			{
				case GL_UNSIGNED_BYTE:
					nIntensity = (unsigned char)(fIntensity*255 + 0.5f);
					for(i=0; i<m_nChannels; i++)
						((unsigned char *)m_pBuffer)[n++] = nIntensity;
					break;
				case GL_FLOAT:
					for(i=0; i<m_nChannels; i++)
						((float *)m_pBuffer)[n++] = fIntensity;
					break;
			}
		}
	}
}
