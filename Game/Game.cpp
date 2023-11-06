#include "Game.h"



{

}

Entity::~Entity()
{

}


void Game::Update() 
{
	// create new ennemy each 5sec
	if (mTimer + 5 < mGameTimer.TotalTime())
	{
		//spawn an ennemy
		mTimer = mGameTimer.TotalTime();
	}	
}

//void main()
//{
//	while (windowIsOpen)
//	{
//		initialise window;
//		clear
//		draw
//		destroy
//
//	}
//}