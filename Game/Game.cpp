#include "Game.h"
#include "Missile.h"

Game::Game(HWND hWnd) : engine(hWnd)
{
    mGameTimer.Reset();
    mTimer = mGameTimer.TotalTime();

    //speed = 2.0f;

    //// Créer une instance de Missile
    //missile.setPosition(cam.GetPosition());
    //missile.setDirection(cam.GetPosition()); 
    //missile.setVelocity(speed);
}

//cam.GetPosition(), cam.GetLookAt(), speed

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

//bool temp = false;
void Game::Update()
{
    mGameTimer.Tick();
    // Update game logic
    engine.Update(mGameTimer);
    /*if (temp == false)
    {
        engine.CreateCube(0.1, 0.1, 0.1, 1, 0, 0);
        engine.CreateCube(0.1, 0.1, 0.1, -1, 0, 0);
        engine.CreateCube(0.1, 0.1, 0.1, 1, 1, 1);
        temp = true;
    }*/
    // create new ennemy each 5sec
    if (mTimer + 0.1 < mGameTimer.TotalTime())
    {
        float posX = rand() % 40 - 20;
        float posY = rand() % 40 - 20;
        float posZ = rand() % 10 + 20;
        //spawn an ennemy
        engine.CreateCube(1.0, 1.0, 1.0, posX, posY, posZ);
        std::wstring str = std::to_wstring(posX);
        OutputDebugString(str.c_str());
        mTimer = mGameTimer.TotalTime();
    }

    //missile.Update(mGameTimer.DeltaTime());
}

void Game::Draw()
{
    // Draw the frame
    engine.Draw();
}