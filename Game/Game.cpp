#include "Game.h"

Game::Game(HWND hWnd) : engine(hWnd)
{
    mHWnd = hWnd;
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
            CalculateFrameStats();
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
        //std::wstring str = std::to_wstring(posX);
        //OutputDebugString(str.c_str());
        mTimer = mGameTimer.TotalTime();
    }
}

void Game::Draw()
{
    // Draw the frame
    engine.Draw();
}

void Game::CalculateFrameStats()
{
    // Code computes the average frames per second, and also the 
    // average time it takes to render one frame.  These stats 
    // are appended to the window caption bar.

    static int frameCnt = 0;
    static float timeElapsed = 0.0f;

    frameCnt++;

    // Compute averages over one second period.
    if ((mGameTimer.TotalTime() - timeElapsed) >= 1.0f)
    {
        float fps = (float)frameCnt; // fps = frameCnt / 1
        float mspf = 1000.0f / fps;

        wstring fpsStr = to_wstring(fps);
        wstring mspfStr = to_wstring(mspf);

        wstring windowText = L"mMainWndCaption    fps: " + fpsStr +
            L"   mspf: " + mspfStr;

        SetWindowText(mHWnd, windowText.c_str());

        // Reset for next average.
        frameCnt = 0;
        timeElapsed += 1.0f;
    }
}