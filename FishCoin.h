#include "Header.h"

/**
 打中鱼后飞出的金币
 */

class FishCoin : public jai::MyNode
{
public:
    
    /**
     @param type 类型 0-银币 1-金币
     @param from 出现的位置
     @param to 目的位置
     */
    FishCoin(int type, const CC::Vec2 & from, const CC::Vec2 & to);
};

/**
 横向排列的多个金币
 */

class FishCoinMulti : public jai::MyNode
{
public:
    /**
     @param type 币的类型 0-银币，1-金币
     @param count 币的数量
     @param from 出现的位置
     @param to 目的位置
     */
    FishCoinMulti(int type, int count, const CC::Vec2 & from, const CC::Vec2 & to);
};
