#include "Game.h"

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

bool temp = false;
void Game::Update()
{
    mGameTimer.Tick();
    // Update game logic
    engine.Update();
    if (temp == false)
    {
        engine.CreateCube(0.1, 0.1, 0.1, 1, 0, 0);
        engine.CreateCube(0.1, 0.1, 0.1, -1, 0, 0);
        engine.CreateCube(0.1, 0.1, 0.1, 1, 1, 1);
        temp = true;
    }
    // create new ennemy each 5sec
    if (mTimer + 5 < mGameTimer.TotalTime())
    {
        //spawn an ennemy
        mTimer = mGameTimer.TotalTime();
    }
}

void Game::Draw()
{
    // Draw the frame
    engine.Draw();
}
