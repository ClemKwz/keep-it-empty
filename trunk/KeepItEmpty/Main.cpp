//////////////////////////////////////////////////////////////////////////////////////////
//	                                Keep It Empty                                       //
//                        Copyright(c) 2012 by Cl�ment Kawczak                          //
//////////////////////////////////////////////////////////////////////////////////////////

#include "hge.h"
#include "Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        Game* game = new Game();
		game->Start();

        return 0;
}