//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Clément Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#include "Player.h"
#include "Game.h"
#include <math.h>
#include "Conf.h"
#include "Fonctions.h"

Player::Player(Game* pGame)
{
	m_pGame = pGame;
	m_eState = Ready;
	m_fPosX = 0.0;
	m_fPosY = 0.0;
	m_nRadius = 7;
	m_fTime = 0.0;
	m_dwColor = 0x7F0D5382;
	m_iTypeClick = 0;
}

void Player::Update()
{
	if(m_eState == Ready)
	{
		m_pGame->GetHGE()->Input_GetMousePos(&m_fReadyPosX, &m_fReadyPosY);

		if(!m_pGame->GetLevel()->GetFirstExplosion() && m_pGame->GetHGE()->Input_GetKeyState(HGEK_RBUTTON) && m_pGame->GetLevel()->GetState() == Ready)
		{
			m_eState = Explode;
			m_fPosX = m_fReadyPosX;
			m_fPosY = m_fReadyPosY;
			m_pGame->GetPlayer()->SetType(2);
			m_iTypeClick = 2;
		}
		if(m_pGame->GetHGE()->Input_GetKeyState(HGEK_LBUTTON) && m_pGame->GetLevel()->GetState() == Ready)
		{
			m_eState = Explode;
			m_fPosX = m_fReadyPosX;
			m_fPosY = m_fReadyPosY;
			m_pGame->GetPlayer()->SetType(1);
			m_iTypeClick = 1;
			m_pGame->GetLevel()->SetFirstExplosion(false);
		}
	}
	else if(m_eState == Explode)
	{
		if(m_nRadius < 50)
			m_nRadius += 2;
		else
		{
			float dt = m_pGame->GetHGE()->Timer_GetDelta();
			m_fTime += dt;

			if(m_fTime > m_pGame->fDeathTimePlayer)
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

void Player::Restart()
{
	m_eState = Ready;
	m_fTime = 0.0;
	m_nRadius = 7;
}

void Player::Draw()
{
	if(m_eState == Ready && m_pGame->GetLevel()->GetState() == Ready)
	{
		m_pGame->GetElementSprite()->SetColor(m_dwColor);
		m_pGame->GetElementSprite()->RenderStretch(m_fReadyPosX - m_nRadius, m_fReadyPosY - m_nRadius, m_fReadyPosX + m_nRadius, m_fReadyPosY + m_nRadius);
	}
	else if(m_eState == Explode || m_eState == Dying)
	{
		m_pGame->GetElementSprite()->SetColor(m_dwColor);
		m_pGame->GetElementSprite()->RenderStretch(m_fPosX - m_nRadius, m_fPosY - m_nRadius, m_fPosX + m_nRadius, m_fPosY + m_nRadius);
	}
	DrawCommand();
}

void Player::DrawCommand()
{
	if(m_eState == Ready && m_pGame->GetLevel()->GetState() == Running)
	{
		m_pGame->GetFont()->SetColor(0xFF00EA17);
		m_pGame->GetFont()->printf(50, 400, HGETEXT_LEFT, "Left click");
		m_pGame->GetFont()->SetColor(0xFF48A1CE);
		m_pGame->GetFont()->printf(50, 450, HGETEXT_LEFT, "Explode balls !");

		if(m_iType == 2)
		{
			m_pGame->GetFont()->SetColor(0xFF00EA17);
			m_pGame->GetFont()->printf(850, 400, HGETEXT_RIGHT, "Right click");
			m_pGame->GetFont()->SetColor(0xFF48A1CE);
			m_pGame->GetFont()->printf(850, 450, HGETEXT_RIGHT, "Multiply balls !");
		}
	}
}

Player::~Player(void)
{
}
