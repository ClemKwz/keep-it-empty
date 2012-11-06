//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Clément Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once

typedef unsigned long DWORD;

class Game;

class Player
{
public:
	enum State {
      Ready,
      Explode,
      Dead
   };

	Player(Game* pGame);
	State GetState(){return m_eState;}
	float GetPosX(){return m_fPosX;}
	float GetPosY(){return m_fPosY;}
	int GetRadius(){return m_nRadius;}
	void Update();
	void Restart();
	void Draw_Circle(float cx, float cy, float Radius, int Segments, DWORD color);
	void Draw();
	~Player(void);

private:
	Game* m_pGame;
	State m_eState;

	float m_fPosX;
	float m_fPosY;

	float m_fReadyPosX;
	float m_fReadyPosY;

	DWORD m_dwColor;

	int m_nRadius;

	// timing the death
	float m_fTime;
};

