//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Cl�ment Kawczak                          //
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
      Dead
   };

	Level(Game* game, int nElements);
	void Update();
	void Restart();
	void Draw();
	~Level(void);

private:
	Game* m_pGame;
	Element** m_ppElements;
	int m_nElements;
};

#endif