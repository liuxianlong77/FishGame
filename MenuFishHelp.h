#pragma once

#include "Header.h"
#include "MenuMidBase.h"

class FishItem : public jai::MyNode
{
public:
    FishItem(int index);
};

class MenuFishHelp : public MenuMidBase
{
    CC::Layer * mContainer;
    CC::extension::ScrollView * mScrollView;
public:
    MenuFishHelp();
};
