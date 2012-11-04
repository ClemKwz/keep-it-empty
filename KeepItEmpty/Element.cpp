//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Clément Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#include "Element.h"
#include <math.h>
#include "Game.h"

Element::Element(Game* pGame, int nPosX, int nPosY)
{
	m_pGame = pGame;
	m_nPosX = nPosX;
	m_nPosY = nPosY;
}

void Element::Draw_Circle(float cx, float cy, float Radius, int Segments, DWORD color)
{
	float EachAngle;
	float a;
	float x1;
	float x2;
	float y1;
	float y2;
 
	EachAngle = 2.0 * M_PI / (float)Segments;
 
	x2 = Radius;
	y2 = 0.0;
 
	for(a=0.0; a<= (2.0*M_PI + EachAngle); a+=EachAngle)
	{
		x1 = x2;
		y1 = y2;
		x2 = Radius * cos(a);
		y2 = Radius * sin(a);
		m_pGame->GetHGE()->Gfx_RenderLine(x1+cx, y1+cy, x2+cx, y2+cy, color);
	}
}

void Element::Draw()
{
	Draw_Circle(m_nPosX, m_nPosY, 15, 30, 0xFFFFFFFF);
}

Element::~Element(void)
{
}
