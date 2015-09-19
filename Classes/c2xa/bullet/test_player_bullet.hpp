/************************************************************************************//**
    @file	c2xa/bullet/test_player_bullet.hpp
    @brief	Test Bullet

    @author    新ゝ月(NewNotMoon)
    @date      2015/08/29
****************************************************************************************/
#ifndef C2XA_BULLET_TEST_PLAYER_BULLET_HPP
#define C2XA_BULLET_TEST_PLAYER_BULLET_HPP

#include <cocos2d.h>

#include <c2xa/c2xa_config.hpp>
#include <c2xa/utility.hpp>

namespace c2xa
{
    namespace bullet
    {
        class test_player_bullet
            : public cocos2d::Node
        {
        private:
            float   duration_ = 320.f; // frame
            float   count_ = 0.f;
            cocos2d::Vec2    target_position_;
            cocos2d::Vec2    first_position_;
            cocos2d::Sprite* bullet_;

        public:
            static test_player_bullet* create( float player_x_ )
            {
                struct once_init
                {
                    once_init()
                    {
                        add_sprite_batch( get_current_scene(), "bomb.png", "player_bomb" );
                    }
                } static once_;
                return create_template<test_player_bullet>( player_x_ );
            }
            virtual bool init( float player_x_ ) // !!! no-override !!!
            {
                if( !Node::init() )
                {
                    return false;
                }
                this->scheduleUpdate();

                target_position_ = { player_x_, app_height }; // とりあえず画面外
                first_position_  = { player_x_, 100 };

                bullet_ = create_sprite_from_batch( get_current_scene(), "player_bomb" );
                bullet_->setPosition( first_position_ );
                addChild( bullet_ );

                return true;
            }
            virtual void update( float delta_ ) override
            {
                count_ += 1.f * ( delta_ * 100 );

                if( count_ > duration_ )
                {
                    bullet_->removeFromParent(); // attention
                    this->removeFromParent();
                }
                else
                {
                    bullet_->setPosition( ( target_position_ - first_position_ ) * count_ / duration_ + first_position_ );
                }
            }
        };
    }
}

#endif//C2XA_BULLET_TEST_PLAYER_BULLET_HPP