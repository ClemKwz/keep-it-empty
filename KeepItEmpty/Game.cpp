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
#include <sstream>
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

	m_pPlayer->SetType(1);
	m_iInitTime = 8;
	m_fTime = (float)m_iInitTime;
	m_dwBackgroundColor = 0;
	m_iCptColor = 0;
	m_nRadiusMax = 50;
	m_bSwitch = true;
	m_pSoundEffects = new HEFFECT[8];

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
			m_fTime = (float)m_iInitTime;
			m_pPlayer->SetType(1);
			delete m_pLevel;
			m_pLevel = new Level(pGame, nElements, 25);
			m_pPlayer->Restart();
			m_nCurrentLevel++;
			if(m_bSwitch)
				fSpeed = (fSpeed/10)*9;
			else
				m_nRadiusMax -= 2;
			m_bSwitch = !m_bSwitch;
			m_dwBackgroundColor = 0;
			m_iCptColor = 0;
		}
	}
	else if(m_pLevel->GetState() == Lost)
	{
		if(m_pHGE->Input_GetKeyState(HGEK_SPACE))
		{
			m_fTime = (float)m_iInitTime;
			m_pPlayer->SetType(1);
			m_pLevel->Restart();
			m_pPlayer->Restart();
			m_dwBackgroundColor = 0;
			m_iCptColor = 0;
		}
	}

	if(m_pPlayer->GetState() >= Explode)
	{
		float dt = m_pHGE->Timer_GetDelta();
		m_fTime -= dt;

		if(m_fTime <= 0)
		{
			m_pPlayer = new Player(this);
			m_pPlayer->SetType(2);
			m_fTime = (float)m_iInitTime;
		}
	}
}

void Game::PlayEffect(int nType)
{
	m_pHGE->Effect_Play(m_pSoundEffects[nType]);
}

bool RenderFunc()
{
	hge->Gfx_BeginScene();
	pGame->Draw();
	hge->Gfx_EndScene();
	
	return false;
}

void Game::Draw()
{
	if(m_pLevel->GetState() == Won && m_iCptColor < 70)
	{
		m_dwBackgroundColor++;
		m_iCptColor++;
	}
	if(m_pLevel->GetState() == Lost && m_iCptColor < 70)
	{
		m_dwBackgroundColor += 0x00020000;
		m_iCptColor++;
	}

	m_pHGE->Gfx_Clear(m_dwBackgroundColor);
	m_pPlayer->Draw();
	m_pLevel->Draw();

	// Draw time counter
	if(m_pLevel->GetState() == Ready)
	{
		int iTmpTime = (int)m_fTime*1000;
		if(iTmpTime%100 == 0 && m_fTime != m_iInitTime)
		{	
			ostringstream os;
			os << m_fTime;
			string s = os.str();
			string s2 = s.substr(0,4);
			m_pFont->SetColor(0xFF48A1CE);
			m_pFont->printf(730, 450, HGETEXT_CENTER, "New action in");
			m_pFont->SetColor(0xFF00EA17);
			m_pFont->printf(840, 450, HGETEXT_CENTER, s2.c_str());
		}
	}

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

		m_pHGE->Resource_AttachPack("res.paq");
		m_pFont = new hgeFont("font.fnt"); 
		HTEXTURE tex = m_pHGE->Texture_Load("particles.png");
		m_pElementSprite = new hgeSprite(tex, 96, 64, 32, 32);
		m_pElementSprite->SetHotSpot(16,16);
		m_pSoundEffects[0] = m_pHGE->Effect_Load("s01.ogg");
		m_pSoundEffects[1] = m_pHGE->Effect_Load("s02.ogg");
		m_pSoundEffects[2] = m_pHGE->Effect_Load("s03.ogg");
		m_pSoundEffects[3] = m_pHGE->Effect_Load("s04.ogg");
		m_pSoundEffects[4] = m_pHGE->Effect_Load("s05.ogg");
		m_pSoundEffects[5] = m_pHGE->Effect_Load("s06.ogg");
		m_pSoundEffects[6] = m_pHGE->Effect_Load("s07.ogg");
		m_pSoundEffects[7] = m_pHGE->Effect_Load("s08.ogg");
		m_pHGE->System_Start();
	}
	else
	{	
		MessageBox(NULL, (LPCWSTR)m_pHGE->System_GetErrorMessage(), L"Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

	for(int i = 0; i < 8;i++)
		m_pHGE->Effect_Free(m_pSoundEffects[i]);
	m_pHGE->System_Shutdown();
    m_pHGE->Release();
}
