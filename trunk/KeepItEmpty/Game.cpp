//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Clément Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "Player.h"
#include "hge.h"

HGE* hge = 0;
Game* pGame = NULL;

Game::Game(void)
{
	pGame = this;

	m_pPlayer = new Player(pGame);

	// Game resolution
	m_nScreenSizeX = 900;
	m_nScreenSizeY = 500;

	m_nCurrentLevel = 0;
	m_ppLevels = new Level*[10];
	m_ppLevels[0] = new Level(pGame, 30);
}

Game::~Game(void)
{
}

bool FrameFunc()
{
  if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
  pGame->Update();
  return false;
}

void Game::Update()
{
	m_ppLevels[m_nCurrentLevel]->Update();
	m_pPlayer->Update();
}

bool RenderFunc()
{
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	pGame->Draw();
	hge->Gfx_EndScene();
	
	return false;
}

void Game::Draw()
{
	m_ppLevels[m_nCurrentLevel]->Draw();
	m_pPlayer->Draw();
}

void Game::Start()
{
	// Initialization
	hge = hgeCreate(HGE_VERSION);
	m_pHGE = hge;
	hge->System_SetState(HGE_LOGFILE, "keep-it-empty.log");
	hge->System_SetState(HGE_SHOWSPLASH, false);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);   
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_USESOUND, true);
	hge->System_SetState(HGE_SCREENWIDTH, m_nScreenSizeX);
    hge->System_SetState(HGE_SCREENHEIGHT, m_nScreenSizeY);
	hge->System_SetState(HGE_TITLE, "Keep It Empty !");
	hge->System_SetState(HGE_HIDEMOUSE, false);

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
