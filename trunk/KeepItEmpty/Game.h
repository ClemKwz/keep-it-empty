//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Clément Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef GAME_H
#define GAME_H
#pragma once

#include "Level.h"
#include "hge.h"

class Player;

class Game
{

public:
	Game(void);
	HGE* GetHGE(){return m_pHGE;}
	void Start();
	int GetScreenSizeX(){return m_nScreenSizeX;}
	int GetScreenSizeY(){return m_nScreenSizeY;}
	Player* GetPlayer(){return m_pPlayer;}
	void Update();
	void Draw();
	~Game(void);

private:
	HGE* m_pHGE;

	Player* m_pPlayer;

	int m_nScreenSizeX;
	int m_nScreenSizeY;

	Level** m_ppLevels;
	int m_nCurrentLevel;

};

#endif

