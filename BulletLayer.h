#pragma once
#include "Header.h"

// 单个炮弹，生命周期从发射开始至爆炸变成鱼网后结束
class Bullet : public jai::MyNode
{
    /**
     @param color 子弹的颜色。0-红色，1-蓝色
     @param type 子弹的类型。0-单子弹，1-双子弹，2-三子弹
     */
    CC::Vec2 mSpeed;
	int mType;
	int mCannonIndex;
	int mBet;
	bool mIsMine;
	bool mIsAlive;
public:
	Bullet(CC::Vec2 from, CC::Vec2 to, bool isMine, int type, int cannonIndex, int bet);

	int getBet() { return mBet; }
	int getCannonIndex() { return mCannonIndex; }

	void die(std::function<void(Bullet*)> onEnd);

	bool isAlive()
	{
		return mIsAlive;
	}
	bool isMine() { return mIsMine; }
};

// 炮弹层，包含所有炮弹
class BulletLayer : public jai::MyLayer
{
	std::map<int/*cannonIndex*/, std::set<Bullet*> > mAllBullets;
public:
    BulletLayer();
	bool isBulletFull(int cannonIndex);
	void addBullet(Bullet * bullet, int cannonIndex);
	void killBullet(Bullet * bullet);
	std::map<int/*cannonIndex*/, std::set<Bullet*> > const & getAll() { return mAllBullets; }
};
