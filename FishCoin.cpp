//
//  FishCoin.cpp
//  casino
//
//  Created by new on 3/15/16.
//
//

#include "FishCoin.h"

namespace Src
{
    using namespace Help;
    
    static AnimationInfo aniCoin[2] = {
        { "image/other/coins.png", 10, 1, 10, true, 0, true },
        { "image/game/fish/coin.png", 10, 1, 10, true, 0, true },
    };
    
    static const float diffX = 66.f;
    static const float speed = 582.f;
}

FishCoin::FishCoin(int type, const CC::Vec2 & from, const CC::Vec2 & to)
{
    CC::Sprite * coin = Help::LoadSprite(Src::aniCoin[type]);
    this->addChild(coin);
    
    auto animation = Help::LoadAnimation(Src::aniCoin[type]);
    animation->setDelayPerUnit(1.f/(Lex::GetRandom().next(20) + 20));
    
    auto animate = CC::RepeatForever::create(CC::Animate::create(animation));
    coin->runAction(animate);

    float dt = sqrtf(powf(from.x - to.x, 2.f) + powf(from.y - to.y, 2)) / Src::speed;
    auto move = CC::Sequence::create(CC::MoveBy::create(0.15f, CC::Vec2(0.f, 44.f)),
                                     CC::MoveBy::create(0.15f, CC::Vec2(0.f, -44.f)),
                                     CC::MoveBy::create(0.12f, CC::Vec2(0.f, 24.f)),
                                     CC::MoveBy::create(0.12f, CC::Vec2(0.f, -24.f)),
                                     CC::MoveBy::create(0.1f, CC::Vec2(0.f, 12.f)),
                                     CC::MoveBy::create(0.1f, CC::Vec2(0.f, -12.f)),
                                     CC::MoveTo::create(dt, to), NULL);
    
    this->setPosition(from);
    this->runAction(move);
    Au->play(FishSound::coin);
    
    this->scheduleOnce([this](float){
        this->removeFromParent();
    }, dt + 0.8f, "OnRemove");
}

FishCoinMulti::FishCoinMulti(int type, int count, const CC::Vec2 & from, const CC::Vec2 & to)
{
    float startX = from.x - (count / 2.f - 0.5f) * Src::diffX;
    for (int i = 0; i < count; ++i)
    {
        auto coin = new FishCoin(type, CC::Vec2(startX + Src::diffX * i, from.y), to);
        this->addChild(coin);
    }
    
    float dt = sqrtf(powf(from.x - to.x, 2.f) + powf(from.y - to.y, 2)) / Src::speed;
    this->scheduleOnce([this](float){
        this->removeFromParent();
    }, dt + 1.f, "OnRemove");
}
