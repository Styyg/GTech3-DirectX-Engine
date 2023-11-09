#include "Enemy.h"

Enemy::Enemy(float birthTime)
{
    AddCollider();
    mBirthTime = birthTime;
}

Enemy::~Enemy()
{

}

void Enemy::Update(GameTimer mGameTimer)
{
    float move = 0.05 * cosf((mGameTimer.TotalTime() - mBirthTime) * 0.8);
    float move2 = 0.05 * sinf((mGameTimer.TotalTime() - mBirthTime) * 0.8);
    mTransform.TranslateInWorld(move, move2, 0);
    /*mEnemiesList[i]->mTransform.SetPosition(move, move2, 6);*/
}