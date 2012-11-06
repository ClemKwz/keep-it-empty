//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Clément Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#include "Level.h"
#include "Game.h"
#include "Player.h"
#include "Conf.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Level::Level(Game* game, int nElement)
{
	m_pGame = game;
	m_nElements = nElement;
	m_ppElements = new Element*[nElement];
	int nSizeX = m_pGame->GetScreenSizeX();
	int nSizeY = m_pGame->GetScreenSizeY();
	for(int i = 0;i < nElement;i++)
	{
		int nElementPosX = rand()%(nSizeX - 100) + 50;
		int nElementPosY = rand()%(nSizeY - 100) + 50;
		
		// Radius fixe
		int nRadius = 7;
		// Radius aléatoire
		//int nRadius = rand()%(15 - 10) + 10; 

		m_ppElements[i] = new Element(m_pGame, nElementPosX, nElementPosY, nRadius, fSpeed);
	}
}

float Square(float x)
{
	return x*x;
}

void Level::Restart()
{
	int nSizeX = m_pGame->GetScreenSizeX();
	int nSizeY = m_pGame->GetScreenSizeY();
	for(int i = 0;i < m_nElements;i++)
	{
		delete m_ppElements[i];
		int nElementPosX = rand()%(nSizeX - 100) + 50;
		int nElementPosY = rand()%(nSizeY - 100) + 50;
		int nRadius = 10;
		m_ppElements[i] = new Element(m_pGame, nElementPosX, nElementPosY, nRadius, fSpeed);
	}
}

void Level::Update()
{
	for(int i = 0;i < m_nElements;i++)
	{
		m_ppElements[i]->Update();
		if(m_ppElements[i]->GetState() == Ready)
		{
			if(m_pGame->GetPlayer()->GetState() == Explode)
			{
				float x1 = m_ppElements[i]->GetPosX();
				float y1 = m_ppElements[i]->GetPosY();
				float x2 = m_pGame->GetPlayer()->GetPosX();
				float y2 = m_pGame->GetPlayer()->GetPosY();
				float fDistance = sqrt(Square(x2 - x1) + Square(y2 - y1));
				if(fDistance <= m_pGame->GetPlayer()->GetRadius() + m_ppElements[i]->GetRadius())
				{
					m_ppElements[i]->SetExploded();
				}
			}
			if(m_pGame->GetPlayer()->GetState() == Explode || m_pGame->GetPlayer()->GetState() == Dead)
			{
				float x1 = m_ppElements[i]->GetPosX();
				float y1 = m_ppElements[i]->GetPosY();
				for(int j = 0;j < m_nElements;j++)
				{
					if(i != j && m_ppElements[j]->GetState() == Explode)
					{
						float x2 = m_ppElements[j]->GetPosX();
						float y2 = m_ppElements[j]->GetPosY();
						float fDistance = sqrt(Square(x2 - x1) + Square(y2 - y1));
						if(fDistance <= m_ppElements[i]->GetRadius() + m_ppElements[j]->GetRadius())
						{
							m_ppElements[i]->SetExploded();
						}
					}
				}
			}
		}
	}
}

void Level::Draw()
{
	for(int i = 0;i < m_nElements;i++)
	{
		m_ppElements[i]->Draw();
	}
}

Level::~Level(void)
{
}
