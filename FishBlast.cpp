#include "FishBlast.h"

namespace Src
{
    using namespace Help;
    
    static SpriteInfo spType[3] = {
        {"image/game/fish/dragon_blue.png", defaultRect, Anchor::CB, CC::Vec2::ZERO},
        {"image/game/fish/dragon_yellow.png", defaultRect, Anchor::CB, CC::Vec2::ZERO},
        {"image/game/fish/dragon_golden.png", defaultRect, Anchor::CB, CC::Vec2::ZERO},
    };
    
    static SpriteInfo spWave = {
        "image/game/fish/wave.png", defaultRect, Anchor::CE, CC::Vec2::ZERO
    };
}

static const float PI = 3.1415926f;
static const float RAD = PI / 180.f;

class FishBlastRay : public jai::MyNode
{
public:
    FishBlastRay(int type, float angle, float variety)
    {
        auto ray = Help::LoadSprite(Src::spType[type]);
        ray->setRotation(angle);
        ray->setOpacity(0xa4);
        this->addChild(ray);
        
        float myScale = variety;
        ray->setScale(myScale);
        
        float x = sinf(angle * RAD) ;
        float y = cosf(angle * RAD) ;
        
        CC::Vec2 pos;
        pos.x = x * Lex::GetRandom().nextFloat() * 34.f * variety;
        pos.y = y * Lex::GetRandom().nextFloat() * 34.f * variety;
        ray->setPosition(CC::Vec2(x, y));

        static const float s = 400.f;
        float dt = 0.5f / variety;
        
        auto moveBy = CC::MoveBy::create(dt, CC::Vec2(x * s, y * s));
        auto fadeTo = CC::FadeTo::create(dt, 0x0);
        auto scaleTo = CC::ScaleTo::create(dt, 1.3f);
        auto spawn = CC::Spawn::create(moveBy, fadeTo, scaleTo, nullptr);
        ray->runAction(spawn);
    }
};

FishBlast::FishBlast(int type)
{
    auto wave = Help::LoadSprite(Src::spWave);
    wave->setScale(0.f);
    this->addChild(wave);
    
    auto scaleTo = CC::ScaleTo::create(0.3f, 4.f);
    auto fadeTo = CC::FadeTo::create(0.3f, 0x0);
    auto spawn = CC::Spawn::create(scaleTo, fadeTo, NULL);
    wave->runAction(spawn);
    
    for (int i = 0; i < 30; ++i)
    {
        float variety = 0.6f + Lex::GetRandom().nextFloat() * 0.4f;
        this->addChild(new FishBlastRay(type, 12.f * i, variety));
    }
    
    this->scheduleOnce([this, type](float){
        for (int i = 0; i < 30; ++i)
        {
            float angle = Lex::GetRandom().nextFloat() * 360.f;
            float variety = Lex::GetRandom().nextFloat() * 0.3f + 0.2f;
            this->addChild(new FishBlastRay(type, angle, variety));
        }
    }, 0.1f, "OnSecond");
    
    Au->play(FishSound::rayBlast);

    this->scheduleOnce([this](float){
        this->removeFromParent();
    }, 1.8f, "OnRemove");
}
