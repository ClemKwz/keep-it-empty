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
	Level(Game* game, int nElements);
	void Draw();
	~Level(void);

private:
	Game* m_pGame;
	Element** m_ppElements;
	int m_nElements;
};

#endif