//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Clément Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef GAME_H
#define GAME_H
#pragma once

#include "Level.h"
#include "hge.h"
#include "hgefont.h"
#include <hgesprite.h>
#include <string>

using namespace std;

enum LevelState {
      Running,
      Lost,
	  Won
   };

enum State {
      Ready,
      Explode,
	  Dying,
      Dead
   };

class Player;

class Game
{

public:
	Game(void);
	int SaveScore(string name, int level);
	int LoadScores();
	bool GetShowScores(){return m_bShowScores;}
	bool GetEnterName(){return m_bEnterName;}
	string GetPlayerName(){return m_bPlayerName;}
	HGE* GetHGE(){return m_pHGE;}
	void InitVars();
	void Start();
	int GetScreenSizeX(){return m_nScreenSizeX;}
	int GetScreenSizeY(){return m_nScreenSizeY;}
	int GetCurrentLevel(){return m_nCurrentLevel;}
	Level* GetLevel(){return m_pLevel;}
	hgeFont* GetFont(){return m_pFont;}
	Player* GetPlayer(){return m_pPlayer;}
	hgeSprite* GetElementSprite(){return m_pElementSprite;}
	int GetRadiusMax(){return m_nRadiusMax;}
	void PlayEffect(int nType);
	void Update();
	void Draw();
	~Game(void);

	// Debug
	float fDeathTimePlayer;
	float fDeathTimeElement;
	float fSpeed;
	int nRadius;
	int nElements;

private:
	HGE* m_pHGE;
	hgeFont* m_pFont;
	hgeSprite* m_pElementSprite;
	HEFFECT* m_pSoundEffects;

	DWORD m_dwBackgroundColor;
	int m_iCptColor; // usefull for degraded background color

	Player* m_pPlayer;

	int m_nScreenSizeX;
	int m_nScreenSizeY;

	bool m_bSwitch;

	float m_fTime;
	int m_nRadiusMax;
	int m_iInitTime;

	bool m_bShowScores;
	bool m_bEnterName;
	string m_bPlayerName;

	Level* m_pLevel;
	int m_nCurrentLevel;

};

#endif

