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
	m_nRadius = 7;
	m_fTime = 0.0;
	m_dwColor = 0x7F0D5382;
}

void Player::Update()
{
	if(m_eState == Ready)
	{
		m_pGame->GetHGE()->Input_GetMousePos(&m_fReadyPosX, &m_fReadyPosY);
		if(m_pGame->GetHGE()->Input_GetKeyState(HGEK_LBUTTON))
		{
			m_eState = Explode;
			m_fPosX = m_fReadyPosX;
			m_fPosY = m_fReadyPosY;
		}
	}
	else if(m_eState == Explode)
	{
		if(m_nRadius < 50)
			m_nRadius += 2;

		float dt = m_pGame->GetHGE()->Timer_GetDelta();
		m_fTime += dt;

		// 3 seconds until death
		if(m_fTime > fDeathTimePlayer)
			m_eState = Dead;
	}
}

void Player::Restart()
{
	m_eState = Ready;
	m_fTime = 0.0;
	m_nRadius = 7;
}

float Square3(float x)
{
	return x*x;
}


void Player::Draw_Circle(float cx, float cy, float Radius, int Segments, DWORD color)
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
 
	for(a = 0.0;a <= (2.0*M_PI + EachAngle);a += EachAngle)
	{
		x1 = x2;
		y1 = y2;
		x2 = Radius * cos(a);
		y2 = Radius * sin(a);
		m_pGame->GetHGE()->Gfx_RenderLine(x1+cx, y1+cy, x2+cx, y2+cy, color);
	}

	// Fill element
	for(int i = cx - Radius;i <= cx;i++)
	{
		for(int j = cy - Radius;j <= cy;j++)
		{
			float fDistance = sqrt(Square3(cx - i) + Square3(cy - j));
			if(fDistance <= Radius + 1)
			{
				int tmpi = cx - i;
				int tmpy = cy - j;

				m_pGame->GetHGE()->Gfx_RenderLine(cx - tmpi, cy - tmpy, cx - tmpi+1, cy - tmpy + 1, color);
				m_pGame->GetHGE()->Gfx_RenderLine(cx - tmpi, cy + tmpy, cx - tmpi+1, cy + tmpy + 1, color);
				m_pGame->GetHGE()->Gfx_RenderLine(cx + tmpi, cy - tmpy, cx + tmpi+1, cy - tmpy + 1, color);
				m_pGame->GetHGE()->Gfx_RenderLine(cx + tmpi, cy + tmpy, cx + tmpi+1, cy + tmpy + 1, color);
			}
		}
	}
}

void Player::Draw()
{
	if(m_eState == Ready)
	{
		Draw_Circle(m_fReadyPosX, m_fReadyPosY, (float)m_nRadius, 50, m_dwColor);
	}
	else if(m_eState == Explode)
	{
		Draw_Circle(m_fPosX, m_fPosY, (float)m_nRadius, 50, m_dwColor);
	}
}

Player::~Player(void)
{
}
