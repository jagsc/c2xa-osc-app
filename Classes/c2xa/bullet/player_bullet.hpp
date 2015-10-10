/************************************************************************************//**
    @file	 c2xa/bullet/player_bullet.hpp
    @author  新ゝ月(NewNotMoon)
    @date    created on 2015/09/20
****************************************************************************************/
#ifndef C2XA_BULLET_PLAYER_BULLET_HPP
#define C2XA_BULLET_PLAYER_BULLET_HPP

#include <cocos2d.h>

#include <c2xa/c2xa_config.hpp>
#include <c2xa/utility.hpp>

namespace c2xa
{
    namespace bullet
    {
        class player_bullet
            : public cocos2d::Node
        {
        private:
            float   duration_ = 120.f; // frame
            float   count_    = 0.f;
            cocos2d::Vec2    target_position_;
            cocos2d::Vec2    first_position_;
            cocos2d::Sprite* bullet_;

        public:
            ~player_bullet()
            {
                bullet_->release();
            }
            static player_bullet* create( float player_x_ )
            {
                return create_template<player_bullet>( player_x_ );
            }
            virtual bool init( float player_x_ ) // !!! no-override !!!
            {
                struct once_init
                {
                    once_init()
                    {
                        add_sprite_batch( get_current_scene(), "img/player_bomb.png", "player_bomb" );
                    }
                } static once_;
                if( !Node::init() )
                {
                    return false;
                }
                scheduleUpdate();

                target_position_ = { player_x_, app_height }; // とりあえず画面外
                first_position_  = { player_x_, player_y_position };

                bullet_ = create_sprite_from_batch( get_current_scene(), "player_bomb" );
                bullet_->retain();
                bullet_->setPosition( first_position_ );
                addChild( bullet_ );

                return true;
            }
            virtual void update( float delta_ ) override
            {
                count_ += 1.f * ( delta_ * 100 );

                if( count_ > duration_ )
                {
                    removeFromParent();
                }
                else
                {
                    bullet_->setPosition( ( target_position_ - first_position_ ) * count_ / duration_ + first_position_ );
                }
            }
        };
    }
}

#endif//C2XA_BULLET_PLAYER_BULLET_HPP