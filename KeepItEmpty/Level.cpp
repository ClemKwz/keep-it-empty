//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Clément Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#include "Level.h"
#include "Game.h"
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
		int nRadius = rand()%(15 - 10) + 10; 
		m_ppElements[i] = new Element(m_pGame, nElementPosX, nElementPosY, nRadius, 0.5);
	}
}

void Level::Update()
{
	for(int i = 0;i < m_nElements;i++)
	{
		m_ppElements[i]->Update();
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
