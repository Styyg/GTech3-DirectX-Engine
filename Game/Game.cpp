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

bool temp = false;
void Game::Update()
{
    // Update game logic
    engine.Update();
    if (temp == false)
    {
        engine.CreateCube(0.1, 0.1, 0.1, 1, 0, 0);
        engine.CreateCube(0.1, 0.1, 0.1, -1, 0, 0);
        engine.CreateCube(0.1, 0.1, 0.1, 1, 1, 1);
        temp = true;
    }
}

void Game::Draw()
{
    // Draw the frame
    engine.Draw();
}