#include <AudioEngine.h>

#include <c2xa/scene/title_scene.hpp>
#include <c2xa/scene/manual_scene.hpp>
#include <c2xa/c2xa_config.hpp>

using namespace cocos2d;
using namespace c2xa::scene;
using namespace c2xa::layer;

namespace
{
    class droid
        : public cocos2d::Node
    {
    private:
        bool state_;
        float count_ = 0;
        Sprite* sprite1;
        Sprite* sprite2;

    public:
        CREATE_FUNC( droid );
        virtual bool init() override
        {
            if( !Node::init() )
            {
                return false;
            }
            scheduleUpdate();
            sprite1 = Sprite::create( "img/bugdroid_1.png" );
            sprite2 = Sprite::create( "img/bugdroid_2.png" );
            sprite2->setOpacity( 0 );
            state_ = true;
            sprite1->setPosition( Vec2{ c2xa::app_width + 50, 400 } );
            sprite2->setPosition( Vec2{ c2xa::app_width + 50, 400 } );
            this->addChild( sprite1 );
            this->addChild( sprite2 );
            return true;
        }
        virtual void update( float delta_ ) override
        {
            count_ += delta_ * 100.f;
            if( count_ > 60.f )
            {
                count_ = 0;
                if( state_ )
                {
                    sprite1->setOpacity( 0 );
                    sprite2->setOpacity( 255 );
                    state_ = false;
                }
                else
                {
                    sprite1->setOpacity( 255 );
                    sprite2->setOpacity( 0 );
                    state_ = true;
                }
            }
            auto x = sprite1->getPositionX();
            if( x < -50 )
            {
                x = c2xa::app_width + 50;
            }
            sprite1->setPosition( Vec2{ x - 0.5f * delta_ * 100.f, 400 } );
            sprite2->setPosition( Vec2{ x - 0.5f * delta_ * 100.f, 400 } );
        }
    };
}

bool title_scene::init()
{
    if( !Scene::init() )
    {
        return false;
    }

    auto layar_1 = title_background::create();
    auto layar_2 = title_btn::create();

    this->addChild( layar_1, 1 );
    this->addChild( layar_2, 0 );
    return true;
}

bool title_btn::init()
{
    if( !Layer::init() )
    {
        return false;
    }
    Size s = Director::getInstance()->getVisibleSize();
    auto sprite = Sprite::create( "img/image_start_3.png" );
    sprite->setPosition( Vec2( s.width*.5, s.height*.5 ) );

    auto bgm_id = cocos2d::experimental::AudioEngine::play2d( "sounds/title_bgm.mp3", true, 0.3f, nullptr );

    this->runAction( Sequence::create( DelayTime::create( 7.4f ), CallFunc::create( [ = ]()
    {
        auto btn_1 = MenuItemImage::create(
            "img/btn_1.png",
            "img/btn_1_o.png",
            [ = ]( Ref* p )
            {
                cocos2d::experimental::AudioEngine::play2d( "sounds/button.mp3", false, 0.4f, nullptr );
                static_cast<MenuItemImage*>( p )->runAction( Blink::create( 1.f, 5 ) );
                cocos2d::experimental::AudioEngine::stop( bgm_id );
                Director::getInstance()->replaceScene( TransitionFade::create( 2.0f, scene::manual_scene::create() ) );
            }
        );
        Menu* pMenu = Menu::create( btn_1, NULL );
        pMenu->setPosition( Vec2( s.width*.5, 250 ) );
        this->addChild( pMenu );
        this->addChild( droid::create(), 2 );
    } ), nullptr ) );

    this->addChild( sprite, 0 );

    return true;
}

bool title_background::init()
{
    if( !Layer::init() )
    {
        return false;
    }

    Size pos_s = Director::getInstance()->getVisibleSize();
    auto sprite_1 = Sprite::create( "img/image_start_1.png" );
    auto sprite_2 = Sprite::create( "img/image_start_2.png" );
    auto sprite_3 = Sprite::create( "img/image_start_b.png" );
    sprite_1->setPosition( Vec2( pos_s.width*.5, pos_s.height*.5 ) );
    sprite_2->setPosition( Vec2( pos_s.width*.5, pos_s.height*.5 ) );
    sprite_3->setPosition( Vec2( pos_s.width*.5, pos_s.height*.5 ) );

    sprite_1->setOpacity( 0 );
    sprite_2->setOpacity( 0 );
    this->addChild( sprite_1, 1 );
    this->addChild( sprite_2, 2 );
    this->addChild( sprite_3, 0 );

    auto action_1 = Sequence::create( FadeIn::create( 1.f ), DelayTime::create( 2.5f ), FadeOut::create( 0.3f ), nullptr );
    auto action_2 = DelayTime::create( 4.5f );
    auto action_3 = Sequence::create( action_2, action_1, nullptr );
    auto action_4 = Sequence::create( DelayTime::create( 7.4f ), FadeOut::create( 1.0f ), nullptr );

    sprite_1->runAction( action_1 );
    sprite_2->runAction( action_3 );
    sprite_3->runAction( action_4 );

    return true;
}
