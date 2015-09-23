/************************************************************************************//**
    @file    c2xa/object/enemies.hpp
    @brief    enemies

    @author    óÈñÿãvãM
    @date      2015/09/12
****************************************************************************************/

#ifndef C2XA_OBJECT_ENEMY_HPP
#define C2XA_OBJECT_ENEMY_HPP

#include <cocos2d.h>
#include <c2xa/c2xa_config.hpp>
#include <c2xa/collision.hpp>
#include <c2xa/utility.hpp>

namespace c2xa
{
    namespace object
    {    
        class base_enemy : public cocos2d::Node
        {
        protected :
            int score;
            cocos2d::Vec2 position_;
            cocos2d::Vec2 move_direction_;
            float move_speed_;
            cocos2d::Sprite *enemy_sprite_;
            //collision collision_;
            float time_ = 0.f;

        public:
            //virtual collision get_collision() const = 0;

        protected:
            //â°ï˚å¸ÇÃà⁄ìÆï˚å¸
            enum class x_move_state
            {
                NONE,
                LEFT,
                RIGHT
            } x_move_state_;
            //ècï˚å¸ÇÃà⁄ìÆï˚å¸
            enum class y_move_state
            {
                NONE,
                UP,
                DOWN
            } y_move_state_;

            //íeî≠éÀ
            virtual void fire();
            //à⁄ìÆ
            virtual void move();
            //îjâÛ
            void delete_enemy_node();
        };

        class enemy1 : base_enemy {
        //    //âÊñ ÉTÉCÉYÇÃéÊìæ
        //    cocos2d::Size winSize = cocos2d::Director::sharedDirector()->getWinSize();

        public:
            CREATE_FUNC(enemy1);

        public:
            //èâä˙âªä÷êî
            virtual bool init() override
            {
                using namespace cocos2d;

                if (!Node::init())
                {
                    return false;
                }
                scheduleUpdate();
                setName("enemy");
 
                enemy_sprite_ = Sprite::create( "img/player_bugdroid.png" );
                enemy_sprite_->setPosition(position_);
                enemy_sprite_->setName( "enemy_sprite" );
                addChild(enemy_sprite_);

                //collision_ = create_collision_circul( enemy_sprite_ );

                x_move_state_ = x_move_state::NONE;
                y_move_state_ = y_move_state::DOWN;
                move_speed_ = 1.f;
                return true;
            }
            //ÉAÉbÉvÉfÅ[Égä÷êî
            virtual void update( float delta_ ) override
            {
                if( position_.y < -50.f )
                {
                    delete_enemy_node();
                }
                else
                {
                    move();
                    time_ += ( delta_ * 100 );
                    if( time_ > 100 )
                    {
                        fire();
                        time_ = 0.f;
                    }
                }
            }

            void fire() override;
            void move() override;

            /*collision get_collision() const override
            {
                return collision_;
            }*/
        };
    }
}

#endif//C2XA_OBJECT_ENEMY_HPP