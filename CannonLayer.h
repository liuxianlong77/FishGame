// 单个炮台
class Cannon : public jai::MyNode
{
    /**
     @param color 炮筒的颜色。0-红色，1-蓝色
     @param type 渔网的类型。0-单炮筒，1-双炮筒，2-三炮筒
     */
	bool mIsMine;
	int mIndex; // 0-3, LB, RB, LT, RT
    int mType = 0;
	int mValue = 0;
	CC::Node * mWaitNode = nullptr;
	CC::Node * mCannonNode = nullptr;
    CC::Sprite* mCannon = nullptr;
    CC::Label* mCannonScoreLabel = nullptr;
	FishUserInfo * mUser = nullptr;
	uint32_t mUserID;
public:
	Cannon(bool isMine, int index);

	CC::Vec2 aim(CC::Vec2 to);
	void fire();

	uint32_t getUesrID() { return mUserID; }
	void updateOwner();

	void updateBet();
	void updateUserData(bool info, bool score);
	
	bool isMine() { return mIsMine; }
	int getType() { return mType; }
	int getValue() { return mValue; }
protected:
	void _setCannonType(int type);
    void _setCannonValue(int num);
};

// 炮台层，管理所有炮台
class CannonLayer : public jai::MyLayer
{
	std::map<int, Cannon*> mCannons;
public:
	CannonLayer();

	Cannon * getCannon(int index);
};

