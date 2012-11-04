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
	Element(Game* pGame, int nPosX, int nPosY);
	void Draw_Circle(float cx, float cy, float Radius, int Segments, DWORD color);
	void Draw();
	~Element(void);

private:
	Game* m_pGame;
	int m_nPosX;
	int m_nPosY;
};

#endif

