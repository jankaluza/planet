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
#include "Texture.h"
#include "GL/glu.h"

void CTexture::Init(CPixelBuffer *pBuffer, bool bClamp, bool bMipmap)
{
	Cleanup();
	m_nType = (pBuffer->GetHeight() == 1) ? GL_TEXTURE_1D : GL_TEXTURE_2D;
	glGenTextures(1, &m_nID);
	Bind();
	glTexParameteri(m_nType, GL_TEXTURE_WRAP_S, bClamp ? GL_CLAMP : GL_REPEAT);
	glTexParameteri(m_nType, GL_TEXTURE_WRAP_T, bClamp ? GL_CLAMP : GL_REPEAT);
	glTexParameteri(m_nType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_nType, GL_TEXTURE_MIN_FILTER, bMipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

	if(m_nType == GL_TEXTURE_1D)
	{
		if(bMipmap)
			gluBuild1DMipmaps(GL_TEXTURE_1D, pBuffer->GetChannels(), pBuffer->GetWidth(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
		else
			glTexImage1D(GL_TEXTURE_1D, 0, pBuffer->GetChannels(), pBuffer->GetWidth(), 0, pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
	}
	else
	{
		if(bMipmap)
			gluBuild2DMipmaps(GL_TEXTURE_2D, pBuffer->GetChannels(), pBuffer->GetWidth(), pBuffer->GetHeight(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
		else
			glTexImage2D(GL_TEXTURE_2D, 0, pBuffer->GetChannels(), pBuffer->GetWidth(), pBuffer->GetHeight(), 0, pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
	}
}

void CTexture::Update(CPixelBuffer *pBuffer, int nLevel)
{
	Bind();
	if(m_nType == GL_TEXTURE_1D)
		glTexSubImage1D(GL_TEXTURE_1D, nLevel, 0, pBuffer->GetWidth(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
	else
		glTexSubImage2D(GL_TEXTURE_2D, nLevel, 0, 0, pBuffer->GetWidth(), pBuffer->GetHeight(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
}

void CTexture::InitCopy(int x, int y, int nWidth, int nHeight, bool bClamp)
{
	Cleanup();
	m_nType = (nHeight == 1) ? GL_TEXTURE_1D : GL_TEXTURE_2D;
	glGenTextures(1, &m_nID);
	Bind();
	glTexParameteri(m_nType, GL_TEXTURE_WRAP_S, bClamp ? GL_CLAMP : GL_REPEAT);
	glTexParameteri(m_nType, GL_TEXTURE_WRAP_T, bClamp ? GL_CLAMP : GL_REPEAT);
	glTexParameteri(m_nType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_nType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if(m_nType == GL_TEXTURE_1D)
		glCopyTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, x, y, nWidth, 0);
	else
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, nWidth, nHeight, 0);
}

void CTexture::UpdateCopy(int x, int y, int nWidth, int nHeight, int nOffx, int nOffy)
{
	Bind();
	if(m_nType == GL_TEXTURE_1D)
		glCopyTexSubImage1D(GL_TEXTURE_1D, 0, nOffx, x, y, nWidth);
	else
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, nOffx, nOffy, x, y, nWidth, nHeight);
}
