//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Clément Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "Player.h"
#include "hge.h"
#include "Conf.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

HGE* hge = 0;
Game* pGame = NULL;

void Game::InitVars()
{
	fstream file("conf.txt", ios::in);
	if(file)
		file >>  fDeathTimePlayer >> fDeathTimeElement >> fSpeed >> nElements >> nRadius; 
	else
	{
		fDeathTimePlayer = 3.0;
		fDeathTimeElement = 3.0;
		fSpeed = 5.0;
		nElements = 30;
		nRadius = 7;
	}
	
}

Game::Game(void)
{
	pGame = this;

	// Debug
	InitVars();

	m_pPlayer = new Player(pGame);

	// Game resolution
	m_nScreenSizeX = 900;
	m_nScreenSizeY = 500;

	m_nCurrentLevel = 0;
	m_pLevel = new Level(pGame, nElements, 25);
}

Game::~Game(void)
{
}

bool FrameFunc()
{
  if(hge->Input_GetKeyState(HGEK_ESCAPE))
	  return true;
  pGame->Update();
  return false;
}

void Game::Update()
{
	m_pLevel->Update();
	m_pPlayer->Update();
	if(m_pLevel->GetState() == Won)
	{
		if(m_pHGE->Input_GetKeyState(HGEK_SPACE))
		{
			delete m_pLevel;
			m_pLevel = new Level(pGame, nElements, 25);
			m_pPlayer->Restart();
			m_nCurrentLevel++;
			fSpeed = (fSpeed/10)*9;
		}
	}
	else if(m_pLevel->GetState() == Lost)
	{
		if(m_pHGE->Input_GetKeyState(HGEK_SPACE))
		{
			m_pLevel->Restart();
			m_pPlayer->Restart();
		}
	}
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
	m_pPlayer->Draw();
	m_pLevel->Draw();
}

void Game::Start()
{
	// Initialization
	hge = hgeCreate(HGE_VERSION);
	m_pHGE = hge;
	m_pHGE->System_SetState(HGE_SHOWSPLASH, false);
	m_pHGE->System_SetState(HGE_RENDERFUNC, RenderFunc);   
	m_pHGE->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	m_pHGE->System_SetState(HGE_WINDOWED, true);
	m_pHGE->System_SetState(HGE_USESOUND, true);
	m_pHGE->System_SetState(HGE_SCREENWIDTH, m_nScreenSizeX);
    m_pHGE->System_SetState(HGE_SCREENHEIGHT, m_nScreenSizeY);
	m_pHGE->System_SetState(HGE_TITLE, "Keep It Empty !");
	m_pHGE->System_SetState(HGE_HIDEMOUSE, false);

	// Set FPS
	m_pHGE->System_SetState(HGE_FPS, 30);

	if(m_pHGE->System_Initiate())
	{
		m_pFont = new hgeFont("font1.fnt");
		m_pHGE->System_Start();
	}
	else
	{	
		MessageBox(NULL, (LPCWSTR)m_pHGE->System_GetErrorMessage(), L"Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

	m_pHGE->System_Shutdown();
    m_pHGE->Release();
}
