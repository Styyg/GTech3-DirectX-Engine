#include "Game.h"

Game::Game(HWND hWnd) : mEngine(hWnd), mInput(hWnd)
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

void Game::Update()
{
    mInput.Update();
    mGameTimer.Tick();
    // Update game logic
    mEngine.Update(mGameTimer);
    // create new ennemy each 5sec
    if (mTimer + 1 < mGameTimer.TotalTime())
    {
        float posX = rand() % 40 - 20;
        float posY = rand() % 40 - 20;
        float posZ = rand() % 30 + 45;

        //spawn an ennemy
        Enemy* enemy = new Enemy(mGameTimer.TotalTime());
        mEngine.CreateForm(enemy, 0);
        enemy->mTransform.SetPosition(posX, posY, posZ);
        //mEnemiesList.push_back(enemy);

        /*std::wstring str = std::to_wstring(posZ);
        OutputDebugString(str.c_str());*/
        mTimer = mGameTimer.TotalTime();
    }

    if (mInput.GetKeyState(VK_LBUTTON) == DOWN)
    {
        Missile* missile = new Missile(mEngine.GetCam());
        mEngine.CreateForm(missile, 1,  1, 1, 1.0);
        missile->mTransform.SetPosition(0, 0, 1.0);
        //mEnemiesList.push_back(enemy);
    }

    //missile.Update(mGameTimer.DeltaTime());
}

void Game::Draw()
{
    // Draw the frame
    mEngine.Draw();
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