//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Clément Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef LEVEL_H
#define LEVEL_H
#pragma once


#include "Element.h"

class Game;

class Level
{
public:
	enum State {
      Ready,
      Explode,
	  Dying,
      Dead
   };

	Level(Game* game, int nElements, int nGoal);
	void Update();
	void Restart();
	void Draw();
	void DrawGoal();
	~Level(void);

private:
	Game* m_pGame;
	Element** m_ppElements;
	int m_nElements;
	int m_nScore;
	int m_nGoal;
};

#endif