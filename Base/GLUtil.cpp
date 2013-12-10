// GLUtil.cpp
//

//#include "Master.h"
#define GLH_EXT_SINGLE_FILE
#include "GLUtil.h"

/*
Allocated video memory:
vertex array range = 2.5 MB
pbuffers = 4 MB
*/


CVertexArray CVertex::Array;
CGLUtil g_glUtil;
CGLUtil *CGLUtil::m_pMain = &g_glUtil;

CGLUtil::CGLUtil()
{
	m_pArray = NULL;	
}

CGLUtil::~CGLUtil()
{
}

void CGLUtil::Init()
{
	// Start by storing the current HDC and HGLRC
	//m_hDC = wglGetCurrentDC();
	//m_hGLRC = wglGetCurrentContext();
	
	m_pArray = CVertex::Array.GetBuffer();

	// Finally, initialize the default rendering context
	//InitRenderContext(/*m_hDC, m_hGLRC*/);
}

void CGLUtil::InitRenderContext(/*HDC hDC, HGLRC hGLRC*/)
{
	//wglMakeCurrent(hDC, hGLRC);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, CVector4(0.0f));

	EnableVertexArray();
	EnableNormalArray();

	//wglMakeCurrent(m_hDC, m_hGLRC);
}

void CGLUtil::StopRenderContext()
{
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_CULL_FACE);
}
