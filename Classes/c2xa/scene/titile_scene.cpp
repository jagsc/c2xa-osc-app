#include <c2xa/scene/title_scene.hpp>
#include <c2xa/scene/main_scene.hpp>

using namespace cocos2d;
using namespace c2xa::scene;
using namespace c2xa::layer;

bool title_scene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
//    Size pos_s = Director::getInstance()->getVisibleSize();
//    auto sprite_1 = Sprite::create("img/image_start_3.png");
    auto layar_1 = title_background::create();
    auto layar_2 = title_btn::create();
//    sprite_1->setPosition(Vec2(pos_s.width*.5,pos_s.height*.5));

//    this->addChild(sprite_1,0);
    this->addChild(layar_1,1);
    this->addChild(layar_2,0);
    return true;
}

bool title_btn::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    Size s = Director::getInstance()->getVisibleSize();
    auto sprite = Sprite::create("img/image_start_3.png");
    sprite->setPosition(Vec2(s.width*.5,s.height*.5));

//    auto btn1 = MenuItemImage::create("img/button1.png", "img/button2.png", CC_CALLBACK_1(title_layer::myCallback, this));
    auto btn_1 = MenuItemImage::create(
        "img/btn_1.png",
        "img/btn_1_o.png",
        [this](Ref *pSender){
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f,scene::main_scene::create() ) );
        }
    );
    auto btn_2 = MenuItemImage::create(
        "img/btn_2.png",
        "img/btn_2_0.png",
        [this](Ref *pSender){

        }
    );
    Menu* pMenu = Menu::create(btn_1,btn_2, NULL);
    pMenu->setPosition(Vec2(s.width*.5, s.height*.3));
    pMenu->alignItemsVerticallyWithPadding(30);
    this->addChild(sprite,0);
    this->addChild(pMenu,1);
    return true;
}

bool title_background::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    Size pos_s = Director::getInstance()->getVisibleSize();
    auto sprite = Sprite::create("img/image_start_1.png");
    sprite->setPosition(Vec2(pos_s.width*.5,pos_s.height*.5));

    sprite->setOpacity( 0 );
//    this->addChild(sprite);
//    auto action0 = FadeIn::create(1);
//    auto action1 = FadeOut::create(1);
//    sprite->runAction(action0);
//    sprite->runAction(action1);
    //auto action = Sequence::create(FadeIn::create(0.2f),DelayTime::create( 3.f ),FadeOut::create( 0.2f ));
    return true;
}
