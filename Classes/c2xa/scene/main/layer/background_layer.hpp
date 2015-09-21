/************************************************************************************//**
    @file	c2xa/scene/main_scene/layer/background_layer.hpp
    @author    VTŒŽ(NewNotMoon)
    @date      2015/09/19
****************************************************************************************/
#ifndef C2XA_SCENE_MAIN_SCENE_LAYER_BACKGROUND_LAYER_HPP
#define C2XA_SCENE_MAIN_SCENE_LAYER_BACKGROUND_LAYER_HPP

#include <cocos2d.h>

#include <c2xa/utility.hpp>

namespace c2xa
{
    namespace scene
    {
        namespace main
        {
            namespace layer
            {
                class background_layer
                    : public cocos2d::Layer
                {
                private:
                    cocos2d::Sprite* bg1_;
                    cocos2d::Sprite* bg2_;
                    float speed_;

                public:
                    ~background_layer()
                    {
                        bg1_->release();
                        bg2_->release();
                    }
                    CREATE_FUNC( background_layer );
                    virtual bool init() override
                    {
                        using namespace cocos2d;
                        if( !Layer::init() )
                        {
                            return false;
                        }

                        speed_ = 1.f;

                        scheduleUpdate();
                        setName( "background_layer" );

                        add_sprite_batch( this, "img/main_background.png", "main_background" );

                        bg1_ = create_sprite_from_batch( this, "main_background" );
                        bg1_->retain();
                        bg1_->setTag( 1 );
                        bg1_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
                        bg1_->setPosition( Point( 0, 0 ) );
                        addChild( bg1_ );

                        bg2_ = create_sprite_from_batch( this, "main_background" );
                        bg2_->retain();
                        bg2_->setTag( 2 );
                        bg2_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
                        bg2_->setPosition( Point( 0, bg1_->getContentSize().height ) );
                        addChild( bg2_ );

                        return true;
                    }
                    virtual void update( float delta_ ) override
                    {
                        bg1_->setPositionY( bg1_->getPositionY() - ( speed_ * delta_ * 100 ) );
                        bg2_->setPositionY( bg2_->getPositionY() - ( speed_ * delta_ * 100 ) );
                        if( std::abs( bg1_->getPositionY() ) >= bg1_->getContentSize().height )
                        {
                            bg1_->setPositionY( bg2_->getPositionY() + bg2_->getContentSize().height );
                        }
                        if( std::abs( bg2_->getPositionY() ) >= bg2_->getContentSize().height )
                        {
                            bg2_->setPositionY( bg1_->getPositionY() + bg1_->getContentSize().height );
                        }
                    }
                    float get_speed() const
                    {
                        return speed_;
                    }
                    void set_speed( float s_ )
                    {
                        speed_ = s_;
                    }
                };
            }
        }
    }
}
#endif//C2XA_SCENE_MAIN_SCENE_LAYER_BACKGROUND_LAYER_HPP
