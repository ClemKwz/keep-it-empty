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
	Element(Game* pGame, int nPosX, int nPosY, int nRadius, float nSpeed);
	void Update();
	void Draw_Circle(float cx, float cy, float Radius, int Segments, DWORD color);
	void Draw();
	~Element(void);

private:
	Game* m_pGame;
	float m_fPosX;
	float m_fPosY;
	int m_nRadius;
	float m_fSpeed;
	float m_fUpdateX;
	float m_fUpdateY;
};

#endif

