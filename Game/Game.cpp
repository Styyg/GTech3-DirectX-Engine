#include "Game.h"

Game::Game(HWND hWnd) : engine(hWnd) {}

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
    // Update game logic
    engine.Update();
    // create new ennemy each 5sec
    if (mTimer + 5 < mGameTimer.TotalTime())
    {
        //spawn an ennemy
        mTimer = mGameTimer.TotalTime();
        std::wstring str = std::to_wstring(mTimer) + L'\n';
        OutputDebugString(str.c_str()); 
    }
}

void Game::Draw()
{
    // Draw the frame
    engine.Draw();
}
