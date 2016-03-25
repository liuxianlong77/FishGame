#include "CannonLayer.hpp"
#include "BulletLayer.hpp"
#include "FishMain.hpp"
#include "FishGameData.hpp"

namespace Src {
    using namespace Help;
    static SpriteInfo cannonInfo[2][3] = {
            {
                { "image/game/fish/cannon/cannon11.png", cocos2d::Rect(0,0,-1.f,-1/2.f), Anchor::CB, CC::Vec2::ZERO},
                { "image/game/fish/cannon/cannon21.png", cocos2d::Rect(0,0,-1.f,-1/2.f), Anchor::CB, CC::Vec2::ZERO},
                { "image/game/fish/cannon/cannon31.png", cocos2d::Rect(0,0,-1.f,-1/2.f), Anchor::CB, CC::Vec2::ZERO},//red cannon
            },
            {
                { "image/game/fish/cannon/cannon12.png", cocos2d::Rect(0,0,-1.f,-1/2.f), Anchor::CB, CC::Vec2::ZERO},
                { "image/game/fish/cannon/cannon22.png", cocos2d::Rect(0,0,-1.f,-1/2.f), Anchor::CB, CC::Vec2::ZERO},
                { "image/game/fish/cannon/cannon32.png", cocos2d::Rect(0,0,-1.f,-1/2.f), Anchor::CB, CC::Vec2::ZERO},//blue cannon
            },
    };
    
    static SpriteInfo baseInfo[2] ={
        { "image/game/fish/base.png", defaultRect, Anchor::CE, CC::Vec2(0,-7) },
        { "image/game/fish/base_bot.png", defaultRect, Anchor::CE, CC::Vec2(0,-7) },
    };//myselef base
    
    static SpriteInfo baseInfo1 ={
        "image/game/fish/base_bot.png", defaultRect, Anchor::CE, CC::Vec2(0,0)
    };//enemy base
    
    static ButtonInfo3 addCanSizeInfo={
        "image/game/fish/base_inc.png", 2, false, defaultRect, Anchor::CE, CC::Vec2(100,-15)
    };
    
    static ButtonInfo3 reduceCanSizeInfo={
        "image/game/fish/base_dec.png", 2, false,  defaultRect, Anchor::CE, CC::Vec2(-100,-15)
    };
    
    static LabelAtlasInfo CanScoreLabelInfo = {
        "image/num/num_13.png" , '0' , 10 , Anchor::CE , CC::Vec2(0,-25)
    };
    
    static SpriteInfo waitInfo = {
        "image/game/fish/waiting.png", CC::Rect(0, 0, -1.f, -1.f), Anchor::CE, CC::Vec2::ZERO
    };
    
	static const int MaxType = 3;
	static const int MaxColor = 2;
    static AnimationInfo fireInfo[MaxColor][MaxType]={
        {
            {"image/game/fish/cannon/cannon11.png" , 1, 2, 2, false, 0.1, true},
            {"image/game/fish/cannon/cannon21.png" , 1, 2, 2, false, 0.1, true},
            {"image/game/fish/cannon/cannon31.png" , 1, 2, 2, false, 0.1, true},
        },
        {
            {"image/game/fish/cannon/cannon12.png" , 1, 2, 2, false, 0.1, true},
            {"image/game/fish/cannon/cannon22.png" , 1, 2, 2, false, 0.1, true},
            {"image/game/fish/cannon/cannon32.png" , 1, 2, 2, false, 0.1, true},
        },
    };
	static const CC::Point cannonPos[4] = {
		CC::Vec2(400, 44),
		CC::Vec2(WIDTH - 400, 44),
		CC::Vec2(400, HEIGHT - 45),
		CC::Vec2(WIDTH - 400, HEIGHT - 45),
	};
}

enum Z_Order {
    Z_Base, Z_Btn, Z_Cannon, Z_Bet, Z_Info
};

Cannon::Cannon(bool self, int index)
{
	mUserID = FishGM->getUserID(index);
	mIsMine = self;
    mIndex = index;

	if(!mIsMine)
	{
		this->addChild(mCannonNode = new jai::MyNode);
		this->addChild(mWaitNode = Help::LoadSprite(Src::waitInfo));
		auto fadeIn = CC::FadeTo::create(1.f, 0xff);
		auto fadeOt = CC::FadeTo::create(1.f, 0x55);
		auto act = CC::Sequence::create(fadeIn, fadeOt, NULL);
		mWaitNode->runAction(CC::RepeatForever::create(act));
	}
	else
	{
		mCannonNode = this;
		mWaitNode = nullptr;
	}

	if(mIsMine)
	{
		auto * addSizeBut = Help::LoadButton(Src::addCanSizeInfo);
		addSizeBut->setCallback([this](Ref* ){
			FishGM->setNextBet();
			this->updateBet();
			this->_setCannonType(FishGM->getCannonType());
		});
		
		CC::MenuItem* reduceSizeBut = Help::LoadButton(Src::reduceCanSizeInfo);
		reduceSizeBut->setCallback([this](Ref* ){
			FishGM->setPrevBet();
			this->updateBet();
			this->_setCannonType(FishGM->getCannonType());
		});
    
		auto menu = Help::CreateMenu();
		menu->addChild(addSizeBut);
		menu->addChild(reduceSizeBut);
		mCannonNode->addChild(menu, Z_Btn);
	}
	mCannonNode->addChild(mCannon=Help::LoadSprite(Src::cannonInfo[mIsMine?0:1][mType]), Z_Cannon);
    mCannonNode->addChild(mCannonScoreLabel = Help::LoadLabel(Src::CanScoreLabelInfo), Z_Bet);
	mCannonScoreLabel->setString("8");

    auto base = Help::LoadSprite(Src::baseInfo[mIsMine ? 0 : 1]);
    mCannonNode->addChild(base, Z_Base);
    
    if (index > 1)
    {
        base->setRotation(180.f);
        base->setPosition(CC::Vec2(0.f, 8.f));
        mCannonScoreLabel->setPosition(CC::Vec2(0.f, 28.f));
        mCannon->setRotation(180.f);
    }
    
    mCannonNode->addChild(mUser = new FishUserInfo(index));//TODO: need a useful uid

	if(!mIsMine)
	{
		mUserID = 0;
		mCannonNode->setVisible(false);
		this->updateOwner();
	}
	else
	{
		mUserID = GET_MY->uid;
	}

	this->updateBet();
}
void Cannon::updateOwner()
{
	if(mIsMine) return;

	if(FishGM->getUserID(mIndex) == mUserID) return;
	mUserID = FishGM->getUserID(mIndex);
	mCannonNode->setVisible(mUserID);
	mWaitNode->setVisible(!mUserID);

	this->updateBet();
	this->updateUserData(true, true);
}

CC::Vec2 Cannon::aim(CC::Vec2 to)
{
	auto direct = to - this->getPosition();
    if ((mIndex <= 1)&&(direct.y < 0)) {
        direct.y = 1;
		to.y = this->getPosition().y + 1;
    }
    if ((mIndex > 1)&&(direct.y > 0)) {
		direct.y = -1;
		to.y = this->getPosition().y - 1;
    }
    float radians =  atan2(direct.y, direct.x);
    float degree = CC_RADIANS_TO_DEGREES(radians);
    mCannon->setRotation(90-degree);
	return to;
}
void Cannon::fire()
{
    if (mIsMine) {
        Au->play(FishSound::bullet);
    }
	auto fireA = CC::Animate::create(Help::LoadAnimation(Src::fireInfo[mIsMine?0:1][mType]));
    auto idle = CC::CallFunc::create([this](void){
		Help::ResetSprite(mCannon, Src::cannonInfo[mIsMine?0:1][mType]);
    });
    mCannon->runAction(CC::Sequence::create(fireA, idle, nullptr));
}
void Cannon::updateUserData(bool info, bool score)
{
	if(!mUserID) return;
	if(info) mUser->updateInfo();
	if(score) mUser->updateScore();
}
void Cannon::updateBet()
{
	if(mIsMine)
	{
		auto fish = FishGameDataMng::Get();
		_setCannonValue(fish->getCurrentBet());
		_setCannonType(fish->getBetIndex());
	}
	else
	{
		auto bIndex = LexRand(FishGM->getBetIndexLimit());
		_setCannonValue(FishGM->getBetForIndex(bIndex));
		_setCannonType(FishGM->betIndex2CannonType(bIndex));
	}
}

void Cannon::_setCannonType(int tt)
{
	if(tt >= Src::MaxType) tt = Src::MaxType - 1;
	if(mType == tt) return;

	mType = tt;
	Help::ResetSprite(mCannon, Src::cannonInfo[mIsMine?0:1][mType]);
}

void Cannon::_setCannonValue(int num)
{
	if(mValue == num) return;
	mValue = num;
    mCannonScoreLabel->setString(jai::NumToString(num));

	if(mIsMine) Au->play(FishSound::connonChange);
}

CannonLayer::CannonLayer()
{
	auto index = FishGameDataMng::Get()->getCannonIndex();
	Cannon * cannon = new Cannon(true, index);
	cannon->setPosition(Src::cannonPos[index]);
	this->addChild(cannon);
	mCannons[index] = cannon;

	for (int i = 0; i < 4; ++i)
	{
		if(i == FishGM->getCannonIndex()) continue;
		Cannon * cannon = new Cannon(false, i);
		cannon->setPosition(Src::cannonPos[i]);
		this->addChild(cannon);
		mCannons[i] = cannon;
	}
}


Cannon * CannonLayer::getCannon(int index)
{
	auto iter = mCannons.find(index);
	if(iter == mCannons.end()) return nullptr;
	return iter->second;
}
