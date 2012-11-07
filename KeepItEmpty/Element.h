//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Clément Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef ELEMENT_H
#define ELEMENT_H
#pragma once

typedef unsigned long DWORD;

class Game;

class Element
{
public:
	enum State {
      Ready,
      Explode,
	  Dying,
      Dead
   };

	Element(Game* pGame, int nPosX, int nPosY, int nRadius, float nSpeed);
	State GetState(){return m_eState;}
	float GetPosX(){return m_fPosX;}
	float GetPosY(){return m_fPosY;}
	int GetRadius(){return m_nRadius;}
	void InitUpdateDirection();
	void InitType();
	void SetExploded();
	void Update();
	void Draw_Circle(float cx, float cy, float Radius, int Segments, DWORD color);
	void Draw();
	~Element(void);

private:
	Game* m_pGame;

	State m_eState;

	float m_fPosX;
	float m_fPosY;

	int m_nRadius;

	float m_fSpeed;

	float m_fUpdateX;
	float m_fUpdateY;

	DWORD m_dwColor;
	int m_nType;

	// timing the death
	float m_fTime;
};

#endif

