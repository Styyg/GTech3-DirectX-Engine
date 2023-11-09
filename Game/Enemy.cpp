#include "Enemy.h"

Enemy::Enemy()
{
    AddCollider();
    mBirthTime;
}

Enemy::~Enemy()
{

}

void Enemy::Update(GameTimer mGameTimer)
{
    float move = 0.05 * cosf((mGameTimer.TotalTime() - mBirthTime) * 0.8);
    float move2 = 0.05 * sinf((mGameTimer.TotalTime() - mBirthTime) * 0.8);

    float rotX = rand() % 360;
    float rotY = rand() % 360;
    float rotZ = rand() % 360;

    mTransform.TranslateInWorld(move, move2, 0);
    /*mEnemiesList[i]->mTransform.SetPosition(move, move2, 6);*/
}

void Enemy::SetBirthTime(float birthTime)
{
    mBirthTime = birthTime;
}
