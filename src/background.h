////////////////////////////////////////////////////////////////////////////
//
// Author:
//   Joakim Eriksson
//
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "animatorfloat.h"
#include <GL/gl.h>

/***************************** D E F I N E S *******************************/
/****************************** M A C R O S ********************************/
/***************************** C L A S S E S *******************************/

////////////////////////////////////////////////////////////////////////////
//
class CBackground
{
public:
					CBackground();
					~CBackground();
	bool			RestoreDevice(CRenderD3D* render);
	void			InvalidateDevice(CRenderD3D* render);
	void			Update(f32 dt);
	bool			Draw(CRenderD3D* render);

	CVectorAnimator	m_RotAnim;			// Used to rotate the background

	bool			CreateTexture(CRenderD3D* render);
        GLuint m_texture;
};

/***************************** I N L I N E S *******************************/
