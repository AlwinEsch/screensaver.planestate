////////////////////////////////////////////////////////////////////////////
//
// Planestate Screensaver for XBox Media Center
// Copyright (c) 2005 Joakim Eriksson <je@plane9.com>
//
// Thanks goes to Warren for his 'TestXBS' program!
//
////////////////////////////////////////////////////////////////////////////
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
////////////////////////////////////////////////////////////////////////////

#include <xbmc/xbmc_scr_dll.h>
#include <GL/gl.h>
#include <SOIL/SOIL.h>
#include "main.h"
#include "planestate.h"
#include "timer.h"
#include <time.h>

CPlanestate* gPlanestate = null;
CRenderD3D gRender;
CTimer*	gTimer = null;

// The probability that we pick a specific configuration. Should sum up to 1.0
f32 gCfgProbability[NUMCFGS] = { 0.35f, 0.35f,0.15f, 0.15f };

////////////////////////////////////////////////////////////////////////////
// XBMC has loaded us into memory, we should set our core values
// here and load any settings we may have from our config file
//
ADDON_STATUS ADDON_Create(void* hdl, void* props)
{
  if (!props)
    return ADDON_STATUS_UNKNOWN;

  SCR_PROPS* scrprops = (SCR_PROPS*)props;

  gRender.m_Width = scrprops->width;
  gRender.m_Height = scrprops->height;

  return ADDON_STATUS_NEED_SETTINGS;
}

////////////////////////////////////////////////////////////////////////////
// XBMC tells us we should get ready to start rendering. This function
// is called once when the screensaver is activated by XBMC.
//
extern "C" void Start()
{
  srand((u32)time(null));
  gPlanestate = new CPlanestate(gCfgProbability);
  if (!gPlanestate)
    return;
  gTimer = new CTimer();
  gTimer->Init();
  if (!gPlanestate->RestoreDevice(&gRender))
    Stop();

  // make sure these add up to 1
  float sum = gCfgProbability[0]+gCfgProbability[1]+gCfgProbability[2]+gCfgProbability[3];
  for (size_t i=0;i<4;++i)
    gCfgProbability[i] /= sum;
}

////////////////////////////////////////////////////////////////////////////
// XBMC tells us to render a frame of our screensaver. This is called on
// each frame render in XBMC, you should render a single frame only - the DX
// device will already have been cleared.
//
extern "C" void Render()
{
  if (!gPlanestate)
    return;
  gTimer->Update();
  gPlanestate->Update(gTimer->GetDeltaTime());
  gPlanestate->Draw(&gRender);
}

////////////////////////////////////////////////////////////////////////////
// XBMC tells us to stop the screensaver we should free any memory and release
// any resources we have created.
//
extern "C" void ADDON_Stop()
{
  if (gPlanestate)
    gPlanestate->InvalidateDevice(&gRender);
  SAFE_DELETE( gPlanestate );
  SAFE_DELETE( gTimer );
}

extern "C" void ADDON_Destroy()
{
}

extern "C" ADDON_STATUS ADDON_GetStatus()
{
  return ADDON_STATUS_OK;
}

extern "C" bool ADDON_HasSettings()
{
  return true;
}

extern "C" unsigned int ADDON_GetSettings(ADDON_StructSetting ***sSet)
{
  return 0;
}

extern "C" ADDON_STATUS ADDON_SetSetting(const char *strSetting, const void *value)
{
  if (strcmp(strSetting, "probability1") == 0)
    gCfgProbability[0] = *(float*)value;
  if (strcmp(strSetting, "probability2") == 0)
    gCfgProbability[1] = *(float*)value;
  if (strcmp(strSetting, "probability3") == 0)
    gCfgProbability[2] = *(float*)value;
  if (strcmp(strSetting, "probability4") == 0)
    gCfgProbability[3] = *(float*)value;

  return ADDON_STATUS_OK;
}

extern "C" void ADDON_FreeSettings()
{
}

extern "C" void ADDON_Announce(const char *flag, const char *sender, const char *message, const void *data)
{
}

extern "C" void GetInfo(SCR_INFO *info)
{
}
