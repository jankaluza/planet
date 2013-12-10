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

#ifndef __Texture_h__
#define __Texture_h__

//#include <GL\GL.h>
#include "../GLee.h"
#include "PixelBuffer.h"

/*******************************************************************************
* Class: CTexture
********************************************************************************
* This class encapsulates OpenGL texture objects. You initialize it with a
* CPixelBuffer instance and a flag indicating whether you want mipmaps to be
* generated.
*******************************************************************************/
class CTexture
{
protected:
	int m_nType;					// GL_TEXTURE_1D or GL_TEXTURE_2D
	unsigned int m_nID;				// OpenGL-generated texture ID

public:

	CTexture()		{ m_nID = -1; }
	CTexture(CPixelBuffer *pBuffer, bool bMipmap=true)
	{
		m_nID = -1;
		Init(pBuffer, bMipmap);
	}
	~CTexture()		{ Cleanup(); }
	void Cleanup()
	{
		if(m_nID != -1)
		{
			glDeleteTextures(1, &m_nID);
			m_nID = -1;
		}
	}

	static void Enable(int nType)			{ glEnable(nType); }
	static void Disable(int nType)			{ glDisable(nType); }
	
	unsigned long GetID()						{ return m_nID; }
	int GetType()						{ return m_nType; }
	void Bind()							{ if(m_nID != -1) glBindTexture(m_nType, m_nID); }
	void Enable()						{ if(m_nID != -1) { Bind(); glEnable(m_nType); } }
	void Disable()						{ if(m_nID != -1) glDisable(m_nType); }

	void Init(CPixelBuffer *pBuffer, bool bClamp=true, bool bMipmap=true);
	void Update(CPixelBuffer *pBuffer, int nLevel=0);

	// Use when rendering to texture (either in the back buffer or a CPBuffer)
	void InitCopy(int x, int y, int nWidth, int nHeight, bool bClamp=true);
	void UpdateCopy(int x, int y, int nWidth, int nHeight, int nOffx=0, int nOffy=0);
};

#endif // __Texture_h__
