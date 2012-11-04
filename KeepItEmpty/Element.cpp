//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Clément Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#include "Element.h"
#include <math.h>
#include "Game.h"

Element::Element(Game* pGame, int nPosX, int nPosY, int nRadius, float fSpeed)
{
	m_pGame = pGame;
	m_fPosX = (float)nPosX;
	m_fPosY = (float)nPosY;
	m_nRadius = nRadius;
	m_fSpeed = fSpeed;

	m_fUpdateX = ((float)(rand()%100))/100;
	m_fUpdateY = ((float)(rand()%100))/100;
	int randX = rand();
	int randY = rand();
	if(randX == 0)
		m_fUpdateX = -m_fUpdateX;
	if(randY == 0)
		m_fUpdateY = -m_fUpdateY;
}

void Element::Update()
{
	m_fPosX += m_fUpdateX * m_fSpeed;
	m_fPosY += m_fUpdateY * m_fSpeed;
	if((m_fPosX <= 0 + m_nRadius/2) || (m_fPosX >= 900 - m_nRadius/2))
		m_fUpdateX = -m_fUpdateX;
	if((m_fPosY <= 0 + m_nRadius/2) || (m_fPosY >= 500 - m_nRadius/2))
		m_fUpdateY = -m_fUpdateY;

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
	Draw_Circle((int)m_fPosX, (int)m_fPosY, m_nRadius, 30, 0xFFFFFFFF);
}

Element::~Element(void)
{
}
