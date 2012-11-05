//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Clément Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#include "Element.h"
#include <math.h>
#include "Game.h"
#include "Conf.h"
#include "hge.h"
#include <iostream>

using namespace std;

Element::Element(Game* pGame, int nPosX, int nPosY, int nRadius, float fSpeed)
{
	m_pGame = pGame;
	m_fPosX = (float)nPosX;
	m_fPosY = (float)nPosY;
	m_nRadius = nRadius;
	m_fSpeed = fSpeed;
	m_eState = Ready;
	m_fTime = 0.0;

	InitUpdateDirection();
}

void Element::InitUpdateDirection()
{
	// 12 values of trigonometric circle
	int random = rand()%3;
	switch(random)
	{
	case 0:
		m_fUpdateX = (float)((sqrt(3.0))/2);
		m_fUpdateY = 0.5;
		break;
	case 1:
		m_fUpdateX = (float)((sqrt(2.0))/2);
		m_fUpdateY = m_fUpdateX;
		break;
	case 2:
		m_fUpdateX = 0.5;
		m_fUpdateY = (float)((sqrt(3.0))/2);
		break;
	default :
		m_fUpdateX = 0.0;
		m_fUpdateY = 1.0;
	}
	random = rand()%2;
	if(random == 1)
		m_fUpdateX = -m_fUpdateX;
	random = rand()%2;
	if(random == 1)
		m_fUpdateY = -m_fUpdateY;
}

void Element::SetExploded()
{
	m_eState = Explode;
	m_nRadius = 50;
}

void Element::Update()
{
	if(m_eState == Ready)
	{
		m_fPosX += m_fUpdateX * m_fSpeed;
		m_fPosY += m_fUpdateY * m_fSpeed;
		if((m_fPosX <= 0 + m_nRadius/2) || (m_fPosX >= m_pGame->GetScreenSizeX() - m_nRadius/2))
			m_fUpdateX = -m_fUpdateX;
		if((m_fPosY <= 0 + m_nRadius/2) || (m_fPosY >= m_pGame->GetScreenSizeY() - m_nRadius/2))
			m_fUpdateY = -m_fUpdateY;
	}
	else if(m_eState == Explode)
	{
		float dt = m_pGame->GetHGE()->Timer_GetDelta();
		m_fTime += dt;

		// 3 seconds until death
		if(m_fTime > fDeathTimeElement)
			m_eState = Dead;
	}
}

void Element::Draw_Circle(float cx, float cy, float Radius, int Segments, DWORD color)
{
	float EachAngle;
	float a;
	float x1;
	float x2;
	float y1;
	float y2;
 
	EachAngle = (float)(2.0 * M_PI) / (float)Segments;
 
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
	if(m_eState != Dead)
		Draw_Circle(m_fPosX, m_fPosY, (float)m_nRadius, 30, 0xFFFFFFFF);
}

Element::~Element(void)
{
}
