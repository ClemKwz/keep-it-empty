//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Clément Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#include "Player.h"
#include "Game.h"
#include <math.h>
#include "Conf.h"

Player::Player(Game* pGame)
{
	m_pGame = pGame;
	m_eState = Ready;
	m_fPosX = 0.0;
	m_fPosY = 0.0;
	m_nRadius = 30;
	m_fTime = 0.0;
}

void Player::Update()
{
	if(m_eState == Ready)
	{
		if(m_pGame->GetHGE()->Input_GetKeyState(HGEK_LBUTTON))
		{
			m_eState = Explode;
			m_pGame->GetHGE()->Input_GetMousePos(&m_fPosX, &m_fPosY);
		}
	}
	else if(m_eState == Explode)
	{
		float dt = m_pGame->GetHGE()->Timer_GetDelta();
		m_fTime += dt;

		// 3 seconds until death
		if(m_fTime > fDeathTimePlayer)
			m_eState = Dead;
	}
}

void Player::Draw_Circle(float cx, float cy, float Radius, int Segments, DWORD color)
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

void Player::Draw()
{
	if(m_eState == Explode)
	{
		Draw_Circle((int)m_fPosX, (int)m_fPosY, 50, m_nRadius, 0xFFFFFFFF);
	}
}


Player::~Player(void)
{
}
