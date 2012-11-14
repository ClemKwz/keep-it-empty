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
	  Dying,
      Dead
	};

	enum LevelState {
      Running,
      Lost,
	  Won
	};

	Player(Game* pGame);
	State GetState(){return m_eState;}
	float GetPosX(){return m_fPosX;}
	float GetPosY(){return m_fPosY;}
	int GetRadius(){return m_nRadius;}
	int GetType(){return m_iType;}
	void SetType(int iType){m_iType = iType;}
	int GetTypeClick(){return m_iTypeClick;}
	void Update();
	void Restart();
	void DrawCommand();
	void Draw();
	~Player(void);

private:
	Game* m_pGame;
	State m_eState;

	int m_iType;
	int m_iTypeClick;

	float m_fPosX;
	float m_fPosY;

	float m_fReadyPosX;
	float m_fReadyPosY;

	DWORD m_dwColor;

	int m_nRadius;

	// timing the death
	float m_fTime;
};

