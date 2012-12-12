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
#include "Fonctions.h"
#include "hgefont.h"

Level::Level(Game* game, int nElement, int nGoal)
{
	m_pGame = game;
	m_nElements = nElement;
	m_nScore = 0;
	m_nGoal = nGoal;
	m_eState = Running;
	m_fTimeCheckLoose = 0.0;
	m_bFirstExplosion = true;
	m_ppElements = new Element*[nElement];
	int nSizeX = m_pGame->GetScreenSizeX();
	int nSizeY = m_pGame->GetScreenSizeY();
	m_bScoreSaved = false;
	for(int i = 0;i < nElement;i++)
	{
		int nElementPosX = rand()%(nSizeX - 100) + 50;
		int nElementPosY = rand()%(nSizeY - 100) + 50;
		
		m_ppElements[i] = new Element(m_pGame, nElementPosX, nElementPosY,m_pGame->nRadius, m_pGame->fSpeed);
	}
}

void Level::Restart()
{
	m_eState = Running;
	int nSizeX = m_pGame->GetScreenSizeX();
	int nSizeY = m_pGame->GetScreenSizeY();
	for(int i = 0;i < m_nElements;i++)
	{
		delete m_ppElements[i];
		int nElementPosX = rand()%(nSizeX - 100) + 50;
		int nElementPosY = rand()%(nSizeY - 100) + 50;
		m_ppElements[i] = new Element(m_pGame, nElementPosX, nElementPosY, m_pGame->nRadius, m_pGame->fSpeed);
	}
	m_nScore = 0;
}

void Level::Update()
{
	for(int i = 0;i < m_nElements;i++)
	{
		m_ppElements[i]->Update();
		if(m_ppElements[i]->GetState() == Ready)
		{
			bool bAlreadyExploded = false;
			bool bReasonToExplode = m_pGame->GetPlayer()->GetState() == Explode || m_pGame->GetPlayer()->GetState() == Dying;
			if(bReasonToExplode)
			{
				float x1 = m_ppElements[i]->GetPosX();
				float y1 = m_ppElements[i]->GetPosY();
				float x2 = m_pGame->GetPlayer()->GetPosX();
				float y2 = m_pGame->GetPlayer()->GetPosY();
				float fDistance = sqrt(Square(x2 - x1) + Square(y2 - y1));
				if(fDistance <= m_pGame->GetPlayer()->GetRadius() + m_ppElements[i]->GetRadius())
				{
					if(m_pGame->GetPlayer()->GetTypeClick() == 1)
					{
						m_ppElements[i]->SetExploded();
						m_nScore++;
					}
					else
					{
						for(int cpt = 0;cpt < m_nElements;cpt++)
						{
							if(m_ppElements[cpt]->GetState() == Dead && !m_ppElements[i]->GetAlreadyMultiply())
							{
								m_ppElements[i]->SetAlreadyMultiply(true);
								m_ppElements[cpt] = new Element(m_pGame, (int)m_ppElements[i]->GetPosX(), (int)m_ppElements[i]->GetPosY(),m_pGame->nRadius, m_pGame->fSpeed);
								for(int cpt2 = cpt+1;cpt2 < cpt+12;cpt2++)
								{
									if(cpt2 < m_nElements)
									{
										if(m_ppElements[cpt2]->GetState() == Dead)
										{
											m_ppElements[cpt2] = new Element(m_pGame, (int)m_ppElements[i]->GetPosX(), (int)m_ppElements[i]->GetPosY(),m_pGame->nRadius, m_pGame->fSpeed);
										}
									}
								}
								m_ppElements[cpt]->SetAlreadyMultiply(true);
								cpt = m_nElements;
							}
						}
					}
					bAlreadyExploded = true;
				}
			}
			if(bReasonToExplode || m_pGame->GetPlayer()->GetState() == Dead || m_pGame->GetPlayer()->GetType() == 2 && !bAlreadyExploded)
			{
				float x1 = m_ppElements[i]->GetPosX();
				float y1 = m_ppElements[i]->GetPosY();
				for(int j = 0;j < m_nElements;j++)
				{
					if(i != j && (m_ppElements[j]->GetState() == Explode || m_ppElements[j]->GetState() == Dying) && !bAlreadyExploded)
					{
						float x2 = m_ppElements[j]->GetPosX();
						float y2 = m_ppElements[j]->GetPosY();
						float fDistance = sqrt(Square(x2 - x1) + Square(y2 - y1));
						if(fDistance <= m_ppElements[i]->GetRadius() + m_ppElements[j]->GetRadius())
						{
							m_ppElements[i]->SetExploded();
							m_nScore++;
							bAlreadyExploded = true;
						}
					}
				}
			}
		}
	}
	if(m_eState == Running)
	{
		if(m_nScore >= m_nGoal)
			m_eState = Won;
		else
		{
			float dt = m_pGame->GetHGE()->Timer_GetDelta();
			m_fTimeCheckLoose += dt;
			if(m_fTimeCheckLoose > 1)
			{
				m_fTimeCheckLoose = 0.0;
				int nCptExploded = 0;
				for(int i = 0;i < m_nElements;i++)
				{
					if(m_ppElements[i]->GetState() == Explode || m_ppElements[i]->GetState() == Dying)
						nCptExploded++;
				}
				if(nCptExploded == 0 && (m_pGame->GetPlayer()->GetState() == Dead || m_pGame->GetPlayer()->GetType() == 2))
					m_eState = Lost;
			}
		}
	}
}

void Level::Draw()
{
	m_pGame->GetFont()->SetColor(0xFF48A1CE);
	m_pGame->GetFont()->printf(790, 5, HGETEXT_LEFT, "Level %d", m_pGame->GetCurrentLevel()+1);
	for(int i = 0;i < m_nElements;i++)
	{
		m_ppElements[i]->Draw();
	}
	DrawGoal();
	if(!m_pGame->GetShowScores())
	{
		if(m_eState == Won)
		{
			m_pGame->GetFont()->SetColor(0xFF00EA17);
			m_pGame->GetFont()->printf(450, 200, HGETEXT_CENTER, "You won !");
			m_pGame->GetFont()->SetColor(0xFF48A1CE);
			m_pGame->GetFont()->printf(450, 250, HGETEXT_CENTER, "Press space bar for next level");
		}
		else if(m_eState == Lost)
		{
			m_pGame->GetFont()->SetColor(0xFF00EA17);
			m_pGame->GetFont()->printf(450, 200, HGETEXT_CENTER, "You lost !");
			m_pGame->GetFont()->SetColor(0xFF48A1CE);
			m_pGame->GetFont()->printf(450, 250, HGETEXT_CENTER, "Press space bar to restart");
		}
		if(m_eState == Won || m_eState == Lost)
		{
			m_pGame->GetFont()->SetColor(0xFF48A1CE);
			m_pGame->GetFont()->printf(450, 300, HGETEXT_CENTER, "Press s to show scores");

			if(!m_bScoreSaved)
			{
				if(!m_pGame->GetEnterName())
					m_pGame->GetFont()->printf(450, 400, HGETEXT_CENTER, "Press enter to save your score");
				else
				{
					m_pGame->GetFont()->printf(450, 400, HGETEXT_RIGHT, "Enter your name : ");
					string s = " ";
					s += m_pGame->GetPlayerName();
					m_pGame->GetFont()->printf(450, 400, HGETEXT_LEFT, s.c_str());
				}
			}
			else
			{
				m_pGame->GetFont()->printf(450, 400, HGETEXT_CENTER, "Score saved !");
			}
		}
	}
}

void Level::DrawGoal()
{
	m_pGame->GetFont()->SetColor(0xFF48A1CE);
	m_pGame->GetFont()->printf(15, 5, HGETEXT_LEFT, "Goal : %d/%d out of %d", m_nScore, m_nGoal, m_nElements);
}

Level::~Level(void)
{
}
