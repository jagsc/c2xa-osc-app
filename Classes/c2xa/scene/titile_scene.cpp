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

    auto layar_1 = title_background::create();
    auto layar_2 = title_btn::create();

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

    this->runAction(Sequence::create(DelayTime::create(7.4f), CallFunc::create([=](){
        auto btn_1 = MenuItemImage::create(
            "img/btn_1.png",
            "img/btn_1_o.png",
            [this](Ref *pSender){
            Director::getInstance()->replaceScene(scene::main_scene::create());
            }
        );
        Menu* pMenu = Menu::create(btn_1, NULL);
        pMenu->setPosition(Vec2(s.width*.5, s.height*.325));
        this->addChild(pMenu);
    }), nullptr));

    this->addChild(sprite,0);

    return true;
}

bool title_background::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    Size pos_s = Director::getInstance()->getVisibleSize();
    auto sprite_1 = Sprite::create("img/image_start_1.png");
    auto sprite_2 = Sprite::create("img/image_start_2.png");
    auto sprite_3 = Sprite::create("img/image_start_b.png");
    sprite_1->setPosition(Vec2(pos_s.width*.5,pos_s.height*.5));
    sprite_2->setPosition(Vec2(pos_s.width*.5,pos_s.height*.5));
    sprite_3->setPosition(Vec2(pos_s.width*.5,pos_s.height*.5));

    sprite_1->setOpacity( 0 );
    sprite_2->setOpacity( 0 );
    this->addChild(sprite_1,1);
    this->addChild(sprite_2,2);
    this->addChild(sprite_3,0);

    auto action_1 = Sequence::create(FadeIn::create(1.f),DelayTime::create( 2.5f ),FadeOut::create( 1.f ),nullptr);
    auto action_2 = DelayTime::create( 4.5f );
    auto action_3 = Sequence::create(action_2,action_1,nullptr);
    auto action_4 = Sequence::create(DelayTime::create( 7.4f ),FadeOut::create( 1.0f ),nullptr);

    sprite_1->runAction(action_1);
    sprite_2->runAction(action_3);
    sprite_3->runAction(action_4);

    return true;
}
