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
	  Dying,
      Dead
   };

	enum LevelState {
      Running,
      Lost,
	  Won
   };

	Level(Game* game, int nElements, int nGoal);
	void Update();
	LevelState GetState(){return m_eState;}
	bool GetFirstExplosion(){return m_bFirstExplosion;}
	void SetFirstExplosion(bool bFirstExplosion){m_bFirstExplosion = bFirstExplosion;}
	void Restart();
	void Draw();
	void DrawGoal();
	~Level(void);

private:
	Game* m_pGame;
	Element** m_ppElements;
	LevelState m_eState;
	int m_nElements;
	int m_nScore;
	int m_nGoal;

	bool m_bFirstExplosion;

	float m_fTimeCheckLoose;
};

#endif