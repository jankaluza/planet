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

#ifndef __PixelBuffer_h__
#define __PixelBuffer_h__

#include "../GLee.h"
//#include "Matrix.h"
#include "Noise.h"
#include <string.h>

#define ALIGN_SIZE		64
#define ALIGN_MASK		(ALIGN_SIZE-1)
#define ALIGN(x)		(((unsigned long)x+ALIGN_MASK) & ~ALIGN_MASK)

typedef enum
{
	UnsignedByteType = GL_UNSIGNED_BYTE,
	SignedByteType = GL_BYTE,
	UnsignedShortType = GL_UNSIGNED_SHORT,
	SignedShortType = GL_SHORT,
	UnsignedIntType = GL_UNSIGNED_INT,
	SignedIntType = GL_INT,
	FloatType = GL_FLOAT,
	DoubleType = GL_DOUBLE
} BufferDataType;

inline const int GetDataTypeSize(const int nDataType)
{
	int nSize;
	switch(nDataType)
	{
		case UnsignedByteType:
		case SignedByteType:
			nSize = 1;
			break;
		case UnsignedShortType:
		case SignedShortType:
			nSize = 2;
			break;
		case UnsignedIntType:
		case SignedIntType:
		case FloatType:
			nSize = 4;
			break;
		case DoubleType:
			nSize = 8;
			break;
		default:
			nSize = 0;
			break;
	}
	return nSize;
}

class C2DBuffer
{
protected:
	int m_nWidth;				// The width of the buffer
	int m_nHeight;				// The height of the buffer
	int m_nDataType;			// The data type stored in the buffer (i.e. GL_UNSIGNED_BYTE, GL_FLOAT)
	int m_nChannels;			// The number of channels of data stored in the buffer
	int m_nElementSize;			// The size of one element in the buffer
	int m_nRowSize;				// The size of one row in the buffer
	void *m_pAlloc;				// The pointer to the pixel buffer
	void *m_pBuffer;			// A byte-aligned pointer (for faster memory access)

public:
	C2DBuffer()						{ m_pAlloc = m_pBuffer = NULL; }
	C2DBuffer(const C2DBuffer &buf)	{ *this = buf; }
	C2DBuffer(const int nWidth, const int nHeight, const int nDataType, const int nChannels=1, void *pBuffer=NULL)
	{
		m_pAlloc = m_pBuffer = NULL;
		Init(nWidth, nHeight, nDataType, nChannels, pBuffer);
	}
	~C2DBuffer()					{ Cleanup(); }

	void operator=(const C2DBuffer &buf)
	{
		Init(buf.m_nWidth, buf.m_nHeight, buf.m_nDataType, buf.m_nChannels);
		memcpy(m_pBuffer, buf.m_pBuffer, GetBufferSize());
	}
	bool operator==(const C2DBuffer &buf)
	{
		return (m_nWidth == buf.m_nWidth && m_nHeight == buf.m_nHeight && m_nDataType == buf.m_nDataType && m_nChannels == buf.m_nChannels);
	}

	void *operator[](const int n)
	{
		return (void *)((unsigned long)m_pBuffer + n * m_nElementSize);
	}
	void *operator()(const int x, const int y)
	{
		return (void *)((unsigned long)m_pBuffer + y * m_nRowSize + x * m_nElementSize);
	}

	void Init(const int nWidth, const int nHeight, const int nDataType, const int nChannels=1, void *pBuffer=NULL)
	{
		// If the buffer is already initialized to the specified settings, then nothing needs to be done
		if(m_pAlloc && m_nWidth == nWidth && m_nHeight == nHeight && m_nDataType == nDataType && m_nChannels == nChannels)
			return;

		Cleanup();
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nDataType = nDataType;
		m_nChannels = nChannels;
		m_nElementSize = m_nChannels * GetDataTypeSize(m_nDataType);
		m_nRowSize = m_nWidth * m_nElementSize;
		if(pBuffer)
			m_pBuffer = pBuffer;
		else
		{
			m_pAlloc = new unsigned char[GetBufferSize() + ALIGN_MASK];
			m_pBuffer = (void *)ALIGN(m_pAlloc);
		}
	}

	void Cleanup()
	{
		if(m_pAlloc)
		{
			delete[] (unsigned char *)m_pAlloc;
			m_pAlloc = m_pBuffer = NULL;
		}
	}

	int GetWidth() const 		{ return m_nWidth; }
	int GetHeight() const		{ return m_nHeight; }
	int GetDataType() const		{ return m_nDataType; }
	int GetChannels() const		{ return m_nChannels; }
	int GetBufferSize() const	{ return m_nHeight * m_nRowSize; }
	void *GetBuffer() const		{ return m_pBuffer; }

	void ClearBuffer()			{ memset(m_pBuffer, 0, GetBufferSize()); }
	void SwapBuffers(C2DBuffer &buf)
	{
		void *pTemp;
		SWAP(m_pAlloc, buf.m_pAlloc, pTemp);
		SWAP(m_pBuffer, buf.m_pBuffer, pTemp);
	}
};

/*******************************************************************************
* Class: CPixelBuffer
********************************************************************************
* This class implements a general-purpose pixel buffer to be used for anything.
* It is often used by CTexture to set up OpenGL textures, so many of the
* parameters you use to initialize it look like the parameters you would pass
* to glTexImage1D or glTexImage2D. Some of the standard pixel buffer routines
* call fast MMX functions implemented in PixelBuffer.asm.
*******************************************************************************/
class CPixelBuffer : public C2DBuffer
{
protected:
	int m_nFormat;				// The format of the pixel data (i.e. GL_LUMINANCE, GL_RGBA)

public:
	CPixelBuffer() : C2DBuffer() {}
	CPixelBuffer(int nWidth, int nHeight, int nChannels=3, int nFormat=GL_RGB, int nDataType=UnsignedByteType) : C2DBuffer(nWidth, nHeight, nDataType, nChannels)
	{
		m_nFormat = nFormat;
	}

	int GetFormat()				{ return m_nFormat; }

	void Init(int nWidth, int nHeight, int nChannels=3, int nFormat=GL_RGB, int nDataType=GL_UNSIGNED_BYTE, void *pBuffer=NULL)
	{
		C2DBuffer::Init(nWidth, nHeight, nDataType, nChannels, pBuffer);
		m_nFormat = nFormat;
	}

	// Miscellaneous initalization routines
	void MakeRandom(unsigned int nSeed);
	void MakeNoise(unsigned long nSeed);
	void MakeGlow(float fExpose=5.75f, float fSizeDisc=0);
};

#endif // __PixelBuffer_h__
