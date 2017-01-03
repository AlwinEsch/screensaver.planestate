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

#include <kodi/screensaver/Screensaver.h>
#include <kodi/General.h>
#include <GL/gl.h>
#include <SOIL/SOIL.h>
#include "main.h"
#include "planestate.h"
#include <string.h>
#include "timer.h"
#include <time.h>

CPlanestate* gPlanestate = null;
CTimer*	gTimer = null;

// The probability that we pick a specific configuration. Should sum up to 1.0
f32 gCfgProbability[NUMCFGS] = { 0.35f, 0.35f,0.15f, 0.15f };

class CScreensaverPlaceState
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstanceScreensaver
{
public:
  CScreensaverPlaceState();

  virtual bool Start() override;
  virtual void Stop() override;
  virtual void Render() override;

private:
  CRenderD3D m_render;
};

////////////////////////////////////////////////////////////////////////////
// Kodi has loaded us into memory, we should set our core values
// here and load any settings we may have from our config file
//
CScreensaverPlaceState::CScreensaverPlaceState()
{
  m_render.m_Width = Width();
  m_render.m_Height = Height();
  
  kodi::GetSettingFloat("probability1", gCfgProbability[0]);
  kodi::GetSettingFloat("probability2", gCfgProbability[1]);
  kodi::GetSettingFloat("probability3", gCfgProbability[2]);
  kodi::GetSettingFloat("probability4", gCfgProbability[3]);
}

////////////////////////////////////////////////////////////////////////////
// Kodi tells us we should get ready to start rendering. This function
// is called once when the screensaver is activated by Kodi.
//
bool CScreensaverPlaceState::Start()
{
  srand((u32)time(null));
  gPlanestate = new CPlanestate(gCfgProbability);
  if (!gPlanestate)
    return false;
  gTimer = new CTimer();
  gTimer->Init();
  if (!gPlanestate->RestoreDevice(&m_render))
    Stop();

  // make sure these add up to 1
  float sum = gCfgProbability[0]+gCfgProbability[1]+gCfgProbability[2]+gCfgProbability[3];
  for (size_t i=0;i<4;++i)
    gCfgProbability[i] /= sum;

  return true;
}

////////////////////////////////////////////////////////////////////////////
// Kodi tells us to stop the screensaver we should free any memory and release
// any resources we have created.
//
void CScreensaverPlaceState::Stop()
{
  if (gPlanestate)
    gPlanestate->InvalidateDevice(&m_render);
  SAFE_DELETE( gPlanestate );
  SAFE_DELETE( gTimer );
}

////////////////////////////////////////////////////////////////////////////
// Kodi tells us to render a frame of our screensaver. This is called on
// each frame render in Kodi, you should render a single frame only - the DX
// device will already have been cleared.
//
void CScreensaverPlaceState::Render()
{
  if (!gPlanestate)
    return;
  gTimer->Update();
  gPlanestate->Update(gTimer->GetDeltaTime());
  gPlanestate->Draw(&m_render);
}

ADDONCREATOR(CScreensaverPlaceState);
