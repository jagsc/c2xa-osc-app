/************************************************************************************//**
    @file	c2xa/bullet/test_player_bullet.hpp
    @brief	Test Bullet

    @author    新ゝ月(NewNotMoon)
    @date      2015/08/29
****************************************************************************************/
#ifndef C2XA_BULLET_TEST_PLAYER_BULLET_HPP
#define C2XA_BULLET_TEST_PLAYER_BULLET_HPP

#include <cocos2d.h>

#include <c2xa/create_template.hpp>
#include <c2xa/c2xa_config.hpp>
#include <c2xa/c2xa_config.hpp>

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

            /*
            Actionは割り込みに弱い(できなくはないが、コールバック地獄になるので危険)
            弾は移動中に接触時に消えるという割り込みの可能性があるため、素直に座標持って自分で動かす
            または、割り込み可能なActionを作るか(作業量増、拡張性増、見通し減)
            */

        public:
            static test_player_bullet* create( float player_x_ )
            {
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

                bullet_ = cocos2d::Sprite::create( "CloseSelected.png" );
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

#endif//C2XA_BULLET_TEST_PLAYER_BULLET_HPP