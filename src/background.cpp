////////////////////////////////////////////////////////////////////////////
//
// Planestate Screensaver for XBox Media Center
// Copyright (c) 2005 Joakim Eriksson <je@plane9.com>
//
////////////////////////////////////////////////////////////////////////////
//
// Just renders a simple cube with a star texture
// The texture used is generated in here so we dont have any dependencies
// on extra bitmaps outside the screensaver
//
////////////////////////////////////////////////////////////////////////////

#include "main.h"
#include "background.h"
#include <memory.h>
#include <SOIL/SOIL.h>

typedef	struct	TBGRenderVertex
{
	f32			x,y,z;
	CRGBA col;
	f32			u, v;
} TBGRenderVertex;

#define	UVSCALE	4.0f
TBGRenderVertex	gBGVertices[] =
{
	// Quad	0
	{-1.0f,	1.0f,-1.0f,	 CRGBA(255, 255, 255, 255), 0.0f, 0.0f	},
	{-1.0f,-1.0f,-1.0f,	 CRGBA(255, 255, 255, 255), UVSCALE, 0.0f },
	{ 1.0f,	1.0f,-1.0f,	 CRGBA(255, 255, 255, 255), 0.0f, UVSCALE },
	{ 1.0f,-1.0f,-1.0f,	 CRGBA(255, 255, 255, 255), UVSCALE, UVSCALE  },

	// Quad	1
	{-1.0f,	1.0f, 1.0f,	 CRGBA(255, 255, 255, 255), 0.0f, 0.0f	},
	{ 1.0f,	1.0f, 1.0f,	 CRGBA(255, 255, 255, 255), UVSCALE, 0.0f },
	{-1.0f,-1.0f, 1.0f,	 CRGBA(255, 255, 255, 255), 0.0f, UVSCALE },
	{ 1.0f,-1.0f, 1.0f,	 CRGBA(255, 255, 255, 255), UVSCALE, UVSCALE },

	// Quad	2
	{-1.0f,	1.0f, 1.0f,	 CRGBA(255, 255, 255, 255), 0.0f, 0.0f	},
	{-1.0f,	1.0f,-1.0f,	 CRGBA(255, 255, 255, 255), UVSCALE, 0.0f },
	{ 1.0f,	1.0f, 1.0f,	 CRGBA(255, 255, 255, 255), 0.0f, UVSCALE },
	{ 1.0f,	1.0f,-1.0f,	 CRGBA(255, 255, 255, 255), UVSCALE, UVSCALE },

	// Quad	3
	{-1.0f,-1.0f, 1.0f,	 CRGBA(255, 255, 255, 255), 0.0f, 0.0f	},
	{ 1.0f,-1.0f, 1.0f,	 CRGBA(255, 255, 255, 255), UVSCALE, 0.0f },
	{-1.0f,-1.0f,-1.0f,	 CRGBA(255, 255, 255, 255), 0.0f, UVSCALE },
	{ 1.0f,-1.0f,-1.0f,	 CRGBA(255, 255, 255, 255), UVSCALE, UVSCALE },

	// Quad	4
	{ 1.0f,	1.0f,-1.0f,	 CRGBA(255, 255, 255, 255), 0.0f, 0.0f	},
	{ 1.0f,-1.0f,-1.0f,	 CRGBA(255, 255, 255, 255), UVSCALE, 0.0f },
	{ 1.0f,	1.0f, 1.0f,	 CRGBA(255, 255, 255, 255), 0.0f, UVSCALE },
	{ 1.0f,-1.0f, 1.0f,	 CRGBA(255, 255, 255, 255), UVSCALE, UVSCALE },

	// Quad	5
	{-1.0f,	1.0f,-1.0f,	 CRGBA(255, 255, 255, 255), 0.0f, 0.0f	},
	{-1.0f,	1.0f, 1.0f,	 CRGBA(255, 255, 255, 255), UVSCALE, 0.0f },
	{-1.0f,-1.0f,-1.0f,      CRGBA(255, 255, 255, 255), 0.0f, UVSCALE},
	{-1.0f,-1.0f, 1.0f,	 CRGBA(255, 255, 255, 255), UVSCALE, UVSCALE  }
};

////////////////////////////////////////////////////////////////////////////
//
CBackground::CBackground() : m_RotAnim(0, CVector(0.0f,	0.0f, 0.0f)), m_texture(0)
{
  m_RotAnim.m_Values[0].SetMinMax(0.0f, 360.0f);	m_RotAnim.m_Values[0].m_AnimMode = AM_RAND;	m_RotAnim.m_Values[0].SetMinMaxITime(20.0f,	40.0f, AM_RAND);
  m_RotAnim.m_Values[1].SetMinMax(0.0f, 360.0f);	m_RotAnim.m_Values[1].m_AnimMode = AM_RAND;	m_RotAnim.m_Values[1].SetMinMaxITime(20.0f,	40.0f, AM_RAND);
  m_RotAnim.m_Values[2].SetMinMax(0.0f, 360.0f);	m_RotAnim.m_Values[2].m_AnimMode = AM_RAND;	m_RotAnim.m_Values[2].SetMinMaxITime(20.0f,	40.0f, AM_RAND);
}

////////////////////////////////////////////////////////////////////////////
//
CBackground::~CBackground()
{
}

////////////////////////////////////////////////////////////////////////////
//
bool	CBackground::RestoreDevice(CRenderD3D* render)
{
  CreateTexture(render);
  return true;
}

////////////////////////////////////////////////////////////////////////////
//
void	CBackground::InvalidateDevice(CRenderD3D* render)
{
  if (m_texture)
    glDeleteTextures(1, &m_texture);
}

////////////////////////////////////////////////////////////////////////////
//
void		CBackground::Update(f32	dt)
{
  m_RotAnim.Update(dt);
}

////////////////////////////////////////////////////////////////////////////
//
bool		CBackground::Draw(CRenderD3D* render)
{
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_texture);

  CVector	r =	m_RotAnim.GetValue();
  glRotatef(r.x, 1.0, 0.0, 0.0);
  glRotatef(r.y, 0.0, 1.0, 0.0);
  glRotatef(r.z, 0.0, 0.0, 1.0);
  glBegin(GL_TRIANGLE_STRIP);
  for (size_t i=0;i<24;++i)
  {
    glColor3f(gBGVertices[i].col.r/255.0,
              gBGVertices[i].col.g/255.0,
              gBGVertices[i].col.b/255.0);
    glTexCoord2f(gBGVertices[i].u, gBGVertices[i].v);
    glVertex3f(gBGVertices[i].x, gBGVertices[i].y, gBGVertices[i].z);
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////
// Creates the background texture
// It's just a bunch of randomized stars
//
bool	CBackground::CreateTexture(CRenderD3D* render)
{
  // Create star texture
  unsigned char* data = new unsigned char[256*256*4];

  memset(data, 0, 256*256*4);
	
  int		pitch	= 256;
  u32*	ptr	= (u32*) data;
  int numStars = 50;
  for	(int i=0; i<numStars; i++)
  {
    s32	x =	Rand(256-2)+1;
    s32	y =	Rand(256-2)+1;
    f32	brightness = RandFloat();
    f32	brightness2 = brightness*RandFloat();

    ptr[x+(y*pitch)] = CRGBA(brightness, brightness, brightness, 1.0f).RenderColor();
    ptr[(x-1)+(y*pitch)] = ptr[(x+1)+(y*pitch)] = ptr[x+((y-1)*pitch)] = ptr[x+((y+1)*pitch)] = CRGBA(brightness2, brightness2, brightness2, 1.0f).RenderColor();
  }

  m_texture = SOIL_create_OGL_texture(data, 256, 256, SOIL_LOAD_RGBA, 0, 0);
  delete[] data;

  return true;
}
