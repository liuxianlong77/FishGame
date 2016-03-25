#include "MenuFishHelp.h"
namespace  Src {
    using namespace Help;
    static float time = 1.0f;
   static AnimationInfo animationInfo[27] = {
        { "image/game/fish/kind/fish_0.png", 6, 1, 6, true, time/6.f, true },
        { "image/game/fish/kind/fish_1.png", 8, 1, 8, true, time/8.f, true },
        { "image/game/fish/kind/fish_2.png", 12, 1, 12, true, time/12.f, true },
        { "image/game/fish/kind/fish_3.png", 12, 1, 12, true, time/12.f, true },
        { "image/game/fish/kind/fish_4.png", 12, 1, 12, true, time/12.f, true },
        { "image/game/fish/kind/fish_5.png", 13, 1, 13, true, time/12.f, true },
        { "image/game/fish/kind/fish_6.png", 18, 1, 18, true, time/12.f, true },
        { "image/game/fish/kind/fish_7.png", 10, 1, 10, true, time/10.f, true },
        { "image/game/fish/kind/fish_8.png", 12, 1, 12, true, time/12.f, true },
        { "image/game/fish/kind/fish_9.png", 12, 1, 12, true, time/12.f, true },
        { "image/game/fish/kind/fish_10.png", 8, 1, 8, true, time/8.f, true },
        { "image/game/fish/kind/fish_11.png", 6, 1, 6, true, time/6.f, true },
        { "image/game/fish/kind/fish_12.png", 12, 1, 12, true, time/12.f, true },
        { "image/game/fish/kind/fish_13.png", 10, 1, 10, true, time/10.f, true },
        { "image/game/fish/kind/fish_14.png", 12, 1, 12, true, time/12.f, true },
        { "image/game/fish/kind/fish_15.png", 12, 1, 12, true, time/12.f, true },
        { "image/game/fish/kind/fish_16.png", 12, 1, 12, true, time/12.f, true },
        { "image/game/fish/kind/fish_17.png", 12, 1, 12, true, time/12.f, true },
        { "image/game/fish/kind/fish_18.png", 7, 1, 7, true, time/7.f, true },
        { "image/game/fish/kind/fish_19.png", 9, 1, 9, true, time/9.f, true },
        { "image/game/fish/kind/fish_20.png", 6, 1, 6, true, time/6.f, true },
        { "image/game/fish/kind/fish_21.png", 11, 1, 11, true, time/11.f, true },
        { "image/game/fish/kind/fish_22.png", 3, 1, 3, true, time/3.f, true },
        { "image/game/fish/kind/fish_23.png", 12, 1, 12, true, time/12.f, true },
        { "image/game/fish/kind/fish_24.png", 1, 1, 1, true, time/1.f, true },
        { "image/game/fish/kind/fish_25.png", 3, 1, 3, true, time/3.f, true },
        { "image/game/fish/kind/fish_26.png", 8, 1, 8, true, time/8.f, true }
    };
    
    static float scaleInfo[27] = {
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.7, 0.7, 0.7, 0.7, 0.7, 0.4, 0.4, 0.4, 0.5, 0.5, 0.3, 0.4, 0.5, 0.5, 0.3
    };
    
    static SpriteInfo helpBgInfo = {
        "image/game/fish/Menu/helpBg.png", defaultRect, Anchor::CE, CC::Vec2::ZERO
    };
    
    static SpriteInfo lightInfo = {
        "image/game/fish/Menu/light.png", defaultRect, Anchor::CE, CC::Vec2::ZERO
    };

    static LabelSystemInfo explainInfo = {
        25, CC::TextHAlignment::CENTER, CC::TextVAlignment::CENTER, GetColor4B(0xffffffff), Anchor::CE, CC::Vec2(0.f, -70.f)
    };
    static std::string strInfo[27] = {
       "小鱼×2", "绿鱼×2", "绿色条纹鱼\n×3", "大眼鱼×4", "热带鱼×5", "尼莫×5", "河豚×6", "蓝鱼×7", "黑白箭鱼×9", "灯笼鱼×8", "乌龟×10",
        
        "蝴蝶鱼×12", "裙摆鱼×15", "金枪鱼×18", "魔鬼鱼×20", "大白鲨×30", "大金鲨×40", "海豚×40\n-100", "大金龙×100\n-200", "黄金鲨×100\n-300","金蟾×300", "李逵×300-\n999", "悟空×300-\n999", "哪吒×300-\n999", "半屏炸弹", "全屏炸弹", "采金船"
    };
    
}
    
static float diffX = 238;
static float diffY = 210;
static float itemY = 180;

static CC::Vec2 contFirstPos(0,659 );
enum Z_Order
{
    Z_Bg, Z_light, Z_fish ,Z_label
};

FishItem::FishItem(int index)
{
    auto bg = Help::LoadSprite(Src::helpBgInfo);
    auto light = Help::LoadSprite(Src::lightInfo);
    auto action = CC::RepeatForever::create( CC::RotateBy::create(5.5f, -360));
    light->runAction(action);
    
    auto fish = Help::LoadSprite(Src::animationInfo[index]);
    fish->setScale(Src::scaleInfo[index]);
    fish->setRotation(90);
    auto animate = CC::Animate::create(Help::LoadAnimation(Src::animationInfo[index]));
    fish->runAction(CC::RepeatForever::create(animate));
    
    auto label = Help::LoadLabel(Src::explainInfo);
    label->setString(Src::strInfo[index]);
    label->setPosition(CC::Vec2(0, -100));
    
    this->addChild(bg, Z_Bg);
    this->addChild(light, Z_Bg);
    this->addChild(fish, Z_Bg);
    this->addChild(label, Z_label);
}

MenuFishHelp::MenuFishHelp()
:MenuMidBase(0, false)
{
    mContainer = CC::Layer::create();
    mContainer->setPosition(CC::Vec2(0, -(itemY*8+40 - 490)));
    mScrollView = CC::extension::ScrollView::create();
    mScrollView->setContainer(mContainer);
    mScrollView->setTouchEnabled(true);
    mScrollView->setViewSize(CC::Size(1030.f, 490.f));
    mScrollView->setContentSize(CC::Size(1500, itemY*8+40));
    mScrollView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    mScrollView->setPosition(153.f, 140.f-40);
    this->addChild(mScrollView);
    
    for (int i = 0; i < 27; i++) {
        auto fish = new FishItem(i);
        fish->setPosition(CC::Vec2(150 + diffX*(i%4) , itemY*8-30 - diffY*(i/4)));
        mContainer->addChild(fish);
    }    
}

