// GLUtil.h : header file
//

#ifndef __GLUtil_h__
#define __GLUtil_h__

//#include "Texture.h"
#include "../GLee.h"
#include "ListTemplates.h"
#include "Matrix.h"

// This must be defined before GL_NV_vertex_array_range
class CVertex;
#define VERTEX_ARRAY_SIZE	65535	// Would use 65536, but unsigned short only goes up to 65535
class CVertexArray : public CStackedArray<CVertex, unsigned short>
{
public:
	// Constructor/destructor methods
	CVertexArray() : CStackedArray<CVertex, unsigned short>(VERTEX_ARRAY_SIZE) {}
	void UpdateElement(unsigned short nElement)	{ SetFlags(nElement, Dirty); }
};

class CVertex
{
// Attributes
public:
	static CVertexArray Array;
	CVector m_vPosition;		// The position of the vertex
	CVector m_vNormal;			// The normal of the vertex (used for smooth shading)
	float m_fTexCoord[4];		// The texture coordinates of the vertex

// Operations
public:
	CVertex()						{}
	CVertex(const CVertex &v)		{ *this = v; }
	void operator=(const CVertex &v)
	{
		m_vPosition = v.m_vPosition;
		m_vNormal = v.m_vNormal;
		m_fTexCoord[0] = v.m_fTexCoord[0];
		m_fTexCoord[1] = v.m_fTexCoord[1];
		m_fTexCoord[2] = v.m_fTexCoord[2];
		m_fTexCoord[3] = v.m_fTexCoord[3];
	}
	void Init(CVector v)
	{
		m_vPosition = v;
		m_vNormal = v;
		m_fTexCoord[0] = m_fTexCoord[1] = 0;
	}
};

class CGLUtil
{
// Attributes
protected:
	// Standard OpenGL members
	//HDC m_hDC;
	//HGLRC m_hGLRC;

	// Members for GL_NV_vertex_array_range and GL_NV_fence extensions
	CVertex *m_pArray;

public:
	static CGLUtil *m_pMain;

// Operations
public:
	CGLUtil();
	~CGLUtil();
	void Init();
	void InitRenderContext(/*HDC hDC=NULL, HGLRC hGLRC=NULL*/);
	void StopRenderContext();

	//HDC GetHDC()					{ return m_hDC; }
	//HGLRC GetHGLRC()				{ return m_hGLRC; }

	int GetVertexArraySize()		{ return VERTEX_ARRAY_SIZE * sizeof(CVertex); }

	void EnableVertexArray()
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(CVertex), &m_pArray->m_vPosition);
	}
	void DisableVertexArray()
	{
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	void EnableNormalArray()
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(CVertex), &m_pArray->m_vNormal);
	}
	void DisableNormalArray()
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	void EnableTextureCoordArray(int nOffset=0)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(CVertex), &m_pArray->m_fTexCoord[nOffset]);
	}
	void DisableTextureCoordArray()
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
};

inline CGLUtil *GLUtil()			{ return CGLUtil::m_pMain; }

#endif // __GLUtil_h__
