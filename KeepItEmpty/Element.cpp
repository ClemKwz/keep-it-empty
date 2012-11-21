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
#include "Fonctions.h"

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
	m_bAlreadyMultiply = false;

	InitUpdateDirection();
	InitType();
}

void Element::InitType()
{
	m_nType = rand()%8;
	switch(m_nType)
	{
	case 0:
		m_dwColor = 0x7FFF6600;
		break;
	case 1:
		m_dwColor = 0x7F23DAFF; 
		break;
	case 2:
		m_dwColor = 0x7F33FF33;
		break;
	case 3:
		m_dwColor = 0x7FFF0202;
		break;
	case 4:
		m_dwColor = 0x7FFF23B2;
		break;
	case 5:
		m_dwColor = 0x7F1C0CFF;
		break;
	case 6:
		m_dwColor = 0x7FE3FF14;
		break;
	case 7:
		m_dwColor = 0x7F7C77FF;
		break;
	default :
		m_dwColor = 0x7FAFFF87;
	}
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
	m_pGame->PlayEffect(m_nType);
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
		if(m_nRadius < m_pGame->GetRadiusMax())
			m_nRadius += 2;
		else
		{
			float dt = m_pGame->GetHGE()->Timer_GetDelta();
			m_fTime += dt;

			if(m_fTime > m_pGame->fDeathTimeElement)
				m_eState = Dying;
		}
	}
	else if(m_eState == Dying)
	{
		if(m_nRadius > 0)
			m_nRadius -= 2;
		else
			m_eState = Dead;
	}
}

void Element::Draw()
{
	if(m_eState != Dead)
	{
		m_pGame->GetElementSprite()->SetColor(m_dwColor);
		m_pGame->GetElementSprite()->RenderStretch(m_fPosX - m_nRadius, m_fPosY - m_nRadius, m_fPosX + m_nRadius, m_fPosY + m_nRadius);
	}
}

Element::~Element(void)
{
}
