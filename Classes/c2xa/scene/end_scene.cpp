/************************************************************************************//**
    @file   c2xa/scene/end_scene.cpp
    @author êVÅTåéÇ©ÇËÇ»
    @date   created on 2015/09/29
****************************************************************************************/
#include <c2xa/scene/end_scene.hpp>
#include <c2xa/scene/title_scene.hpp>

using namespace cocos2d;
using namespace c2xa::scene;

bool end_scene::init()
{
    if( !Scene::init() )
    {
        return false;
    }

    setName( "end_scene" );

    auto sprite_1_ = Sprite::create( "img/end_scene_1.png" );
    auto sprite_2_ = Sprite::create( "img/end_scene_2.png" );
    auto sprite_3_ = Sprite::create( "img/end_scene_3.png" );
    auto sprite_4_ = Sprite::create( "img/end_scene_4.png" );

    sprite_1_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    sprite_2_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    sprite_3_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    sprite_4_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );

    sprite_1_->setOpacity( 0 );
    sprite_2_->setOpacity( 0 );
    sprite_3_->setOpacity( 0 );
    sprite_4_->setOpacity( 0 );

    addChild( sprite_1_ );
    addChild( sprite_2_ );
    addChild( sprite_3_ );
    addChild( sprite_4_ );

    sprite_1_->runAction(
        Sequence::create(
            FadeIn::create( 0.2f ),
            DelayTime::create( 2.5f ),
            FadeOut::create( 0.2f ),
            CallFunc::create( [ = ]
            {
                sprite_2_->runAction(
                    Sequence::create(
                        FadeIn::create( 0.2f ),
                        DelayTime::create( 2.5f ),
                        FadeOut::create( 0.2f ),
                        CallFunc::create( [ = ]
                        {
                            sprite_3_->runAction(
                                Sequence::create(
                                    FadeIn::create( 0.2f ),
                                    DelayTime::create( 3.f ),
                                    FadeOut::create( 0.2f ),
                                    CallFunc::create( [ = ]
                                    {
                                        sprite_4_->runAction(
                                            Sequence::create(
                                                FadeIn::create( 0.2f ),
                                                DelayTime::create( 1.f ),
                                                CallFunc::create( [ = ]
                                                {
                                                    auto dispatcher = Director::getInstance()->getEventDispatcher();
                                                    auto keyboard_listener_ = EventListenerKeyboard::create();
                                                    auto touch_listener_    = EventListenerTouchOneByOne::create();
                                                    keyboard_listener_->onKeyPressed = [ = ]( EventKeyboard::KeyCode key_, Event* event_ )
                                                    {
                                                        Director::getInstance()->replaceScene( TransitionFade::create( 2.0f, scene::title_scene::create() ) );
                                                    };
                                                    touch_listener_->onTouchBegan = [ = ]( Touch* t_, Event* )
                                                    {
                                                        Director::getInstance()->replaceScene( TransitionFade::create( 2.0f, scene::title_scene::create() ) );
                                                        return true;
                                                    };
                                                    dispatcher->addEventListenerWithSceneGraphPriority( touch_listener_, this );
                                                    dispatcher->addEventListenerWithSceneGraphPriority( keyboard_listener_, this );
                                                } ),
                                                nullptr ) );
                                    } ),
                                    nullptr ) );
                        } ),
                        nullptr) );
            } ),
            nullptr) );

    return true;
}