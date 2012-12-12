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
#include "sqlite3.h"
#include <vector>
#include <algorithm>

HGE* hge = 0;
Game* pGame = NULL;

vector<std::pair<string, int>> vScores(0);

bool sortV(pair<string, int> pair1, pair<string, int> pair2)
{
	return pair1.second > pair2.second;
}

static int callbackDB(void *NotUsed, int argc, char **argv, char **azColName)
{
	string name = argv[0];
	int level = atoi(argv[1]);
	std::pair<string, int> pair = std::make_pair(name, level);
	vScores.push_back(pair);
	return 0;
}

int Game::SaveScore(string name, int level)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open("scores.db", &db);
	if(rc)
	{
		sqlite3_close(db);
		return(1);
	}
	std::ostringstream oss;
	oss << level;
	string req = "insert into score values (\"";
	req += name;
	req += "\",";
	req += oss.str();
	req += ");";
	rc = sqlite3_exec(db, req.c_str(), callbackDB, 0, &zErrMsg);

	if(rc != SQLITE_OK)
	{
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
	return 0;
}

int Game::LoadScores()
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open("scores.db", &db);
	if(rc)
	{
		sqlite3_close(db);
		return(1);
	}
	string req = "select * from score;";
	rc = sqlite3_exec(db, req.c_str(), callbackDB, 0, &zErrMsg);

	if(rc != SQLITE_OK)
	{
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
	return 0;
}

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

	m_bShowScores = false;
	m_bEnterName = false;
	m_bPlayerName = "";

	m_nCurrentLevel = 0;
	m_pLevel = new Level(pGame, nElements, 25);
}

Game::~Game(void)
{
}

bool FrameFunc()
{
  pGame->Update();
  return false;
}

void Game::Update()
{
	m_pLevel->Update();
	m_pPlayer->Update();

	if(m_pLevel->GetState() != Running)
	{
		if(!m_bEnterName)
		{
			if(m_pHGE->Input_GetKeyState(HGEK_S) && !m_bShowScores)
			{
				m_bShowScores = true;
				LoadScores();
			}
			if(m_pHGE->Input_GetKeyState(HGEK_ESCAPE) && m_bShowScores)
			{
				m_bShowScores = false;
				vScores.clear();
			}
		}

		int cEnter = 0;
		cEnter = m_pHGE->Input_GetChar();
		if(cEnter == 13 && m_bEnterName) // Enter key
		{
			
			m_bEnterName = false;
			int rc = SaveScore(m_bPlayerName, m_nCurrentLevel+1);
			m_pLevel->SetScoreSaved();
		}	
		else if(cEnter == 13 && !m_bEnterName && !m_pLevel->GetScoreSaved())
		{
			m_bEnterName = true;
		}

		if(m_bEnterName)
		{
			int c = m_pHGE->Input_GetChar();
			if(c != 0 && m_bPlayerName.size() < 10 && !m_pHGE->Input_GetKeyState(HGEK_ENTER))
			{
				if(c != 8)
				m_bPlayerName += c;
			}
			if(c == 8 && m_bPlayerName.size() > 0 && !m_pHGE->Input_GetKeyState(HGEK_ENTER))
					m_bPlayerName.erase(m_bPlayerName.size()-1, m_bPlayerName.size());
		}
	}

	if(!m_bShowScores)
	{
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

	if(m_bShowScores)
	{
		std::sort(vScores.begin(), vScores.end(), sortV); 
		m_pFont->SetColor(0xFF00EA17);
		m_pFont->printf(450, 150, HGETEXT_CENTER, "Highest Scores");
		int y = 200;
		int size;
		if(vScores.size() >= 5)
			size = 5;
		else
			size = vScores.size();
		for(int i = 0;i < size;i++)
		{
			m_pFont->SetColor(0xFF48A1CE);
			string s = vScores[i].first;
			m_pFont->printf(410, (float)y, HGETEXT_RIGHT, s.c_str());
			std::ostringstream oss;
			oss << vScores[i].second;
			m_pFont->printf(490, (float)y, HGETEXT_LEFT, oss.str().c_str());
			y += 40;
		}
		m_pFont->SetColor(0xFF48A1CE);
		m_pFont->printf(450, 450, HGETEXT_CENTER, "Press escape to leave");
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
