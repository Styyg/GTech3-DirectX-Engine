#include "Game.h"
#include "GameObject.h"

Game::Game(HWND hWnd) : engine(hWnd)
{
    mGameTimer.Reset();
    mTimer = mGameTimer.TotalTime();
}

Game::~Game() {}

void Game::Run()
{
    MSG msg = {};

    while (msg.message != WM_QUIT)
    {
        // If there are Window messages then process them.
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // Otherwise, do animation/game stuff.
        else
        {
            Update();
            Draw();
        }
    }
}


void Game::Update()
{
    mGameTimer.Tick();
    // Update game logic
    engine.Update(mGameTimer);
    // create new ennemy each 5sec
    if (mTimer + 1 < mGameTimer.TotalTime())
    {
        float posX = rand() % 40 - 20;
        float posY = rand() % 40 - 20;
        float posZ = rand() % 30 + 15;
        //spawn an ennemy
        GameObject* mGameObject = engine.CreateCube(1.0, 1.0, 1.0, posX, posY, posZ);
        mGameObjectsList.push_back(mGameObject);
        /*std::wstring str = std::to_wstring(posZ);
        OutputDebugString(str.c_str());*/
        mTimer = mGameTimer.TotalTime();
    }
    for (int i = 0; i < mGameObjectsList.size(); i++)
    {
        float move = 0.05*cosf(mGameTimer.TotalTime() * 0.8);
        float move2 = 0.05 *sinf(mGameTimer.TotalTime() * 0.8);
        mGameObjectsList[i]->mTransform.TranslateInWorld(move, move2, 0); 
        /*mGameObjectsList[i]->mTransform.SetPosition(move, move2, 6);*/
    }
}

void Game::Draw()
{
    // Draw the frame
    engine.Draw();
}
