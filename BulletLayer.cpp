#include "BulletLayer.hpp"
#include "FishGameData.hpp"
#include "FishNet.h"

namespace Src {
    using namespace Help;
    static SpriteInfo bulletInfo[2][3] = {
        {
            {"image/game/fish/bullet/bullet11.png" , Help::defaultRect, Help::Anchor::CE, CC::Point(0,0)},
            {"image/game/fish/bullet/bullet21.png" , Help::defaultRect, Help::Anchor::CE, CC::Point(0,0)},
            {"image/game/fish/bullet/bullet31.png" , Help::defaultRect, Help::Anchor::CE, CC::Point(0,0)},//red bullet
        },
        {
            { "image/game/fish/bullet/bullet12.png" , Help::defaultRect, Help::Anchor::CE, CC::Point(0,0) },
            { "image/game/fish/bullet/bullet22.png" , Help::defaultRect, Help::Anchor::CE, CC::Point(0,0) },
            { "image/game/fish/bullet/bullet32.png" , Help::defaultRect, Help::Anchor::CE, CC::Point(0,0) },//blue bullet
        },
    };
};

Bullet::Bullet(CC::Vec2 from, CC::Vec2 to, bool isMine, int type, int cannonIndex, int bet)
{
	mIsAlive = true;
	mBet = bet;
	mCannonIndex = cannonIndex;

	auto direct = (to - from).getNormalized();

	mSpeed = direct * FishGM->getBulletSpeed(cannonIndex);
	mType = type;
	mIsMine = isMine;
	auto bulletImage = Help::LoadSprite(Src::bulletInfo[mIsMine?0:1][mType]);
    this->addChild(bulletImage);
    this->setContentSize(bulletImage->getContentSize());
    this->setPosition(from + direct * 100);
    auto fixDirect = [this]() {
        
        float radians = - atan2(mSpeed.y, mSpeed.x);
        float degree = CC_RADIANS_TO_DEGREES(radians);
        this->setRotation(degree + 90);
    };
    
    fixDirect();
    
    this->schedule([this, fixDirect](float delta) {
        this->setPosition(this->getPosition()+ mSpeed * delta);
        auto pos = this->getPosition();
        bool changed = false;
        if((pos.x < 0 && mSpeed.x < 0) || (pos.x > WIDTH && mSpeed.x > 0))
        {
            mSpeed.x = -mSpeed.x;
            changed = true;
        }
       
        if ((pos.y<0&&mSpeed.y<0)||(pos.y>HEIGHT&&mSpeed.y>0)) {
            mSpeed.y=-mSpeed.y;
            changed=true;
        }
        if(changed)
        {
            fixDirect();
        }
    }, 0.f, "Update");
}

void Bullet::die(std::function<void(Bullet*)> onEnd)
{
   
    mIsAlive = false;
	this->removeAllChildren();
	this->unschedule("Update");
    
	// TODO:变成鱼网
    this->addChild(new FishNet(mIsMine?0:1, mType));
    
	if(onEnd)
	{
		this->scheduleOnce([this, onEnd](float) {
			onEnd(this);
		}, 3.f, "End");
	}
}

BulletLayer::BulletLayer()
{
    
}
bool BulletLayer::isBulletFull(int cIndex)
{
	return mAllBullets[cIndex].size() >= FishGameDataMng::Get()->getMaxBullet(cIndex);
}
void BulletLayer::killBullet(Bullet *bullet)
{
	bullet->die([this](Bullet* b) {
		mAllBullets[b->getCannonIndex()].erase(b);
	});
}
void BulletLayer::addBullet(Bullet * bullet, int cannonIndex)
{
	this->addChild(bullet);
	mAllBullets[cannonIndex].insert(bullet);
}


