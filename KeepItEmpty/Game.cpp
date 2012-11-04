//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Clément Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "hge.h"

HGE* hge = 0;

Game::Game(void)
{
	// Game resolution
	m_nScreenSizeX = 900;
	m_nScreenSizeY = 500;
}

Game::~Game(void)
{
}

bool FrameFunc()
{
  if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
  return false;
}

bool RenderFunc()
{
	return false;
}

void Game::Start()
{
	// Initialization
	hge = hgeCreate(HGE_VERSION);
	hge->System_SetState(HGE_SHOWSPLASH, false);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);   
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_USESOUND, true);
	hge->System_SetState(HGE_SCREENWIDTH, m_nScreenSizeX);
    hge->System_SetState(HGE_SCREENHEIGHT, m_nScreenSizeY);
	hge->System_SetState(HGE_TITLE, "Keep It Empty !");

	if(hge->System_Initiate())
	{
		hge->System_Start();
	}
	else
	{	
		MessageBox(NULL, (LPCWSTR)hge->System_GetErrorMessage(), L"Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

	hge->System_Shutdown();
    hge->Release();
}
