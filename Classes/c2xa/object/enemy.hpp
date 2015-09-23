/************************************************************************************//**
    @file    c2xa/object/enemies.hpp
    @brief    enemies

    @author    鈴木久貴
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
            //横方向の移動方向
            enum class x_move_state
            {
                NONE,
                LEFT,
                RIGHT
            } x_move_state_;
            //縦方向の移動方向
            enum class y_move_state
            {
                NONE,
                UP,
                DOWN
            } y_move_state_;

            //弾発射
            virtual void fire();
            //移動
            virtual void move();
            //破壊
            void delete_enemy_node();
        };

        class enemy1 : base_enemy {
        //    //画面サイズの取得
        //    cocos2d::Size winSize = cocos2d::Director::sharedDirector()->getWinSize();

        public:
            ~enemy1()
            {
                enemy_sprite_->release();
            }
            CREATE_FUNC(enemy1);

        public:
            //初期化関数
            virtual bool init() override
            {
                using namespace cocos2d;

                if (!Node::init())
                {
                    return false;
                }


                struct once_init
                {
                    once_init()
                    {
                        add_sprite_batch( get_current_scene(), "img/player_bugdroid.png", "enemy" );
                    }
                } static once_;

                scheduleUpdate();
                setName( "enemy1" );

                enemy_sprite_ = create_sprite_from_batch( get_current_scene(), "enemy" );
                enemy_sprite_->retain();
                enemy_sprite_->setPosition(position_);
                enemy_sprite_->setName( "enemy_sprite" );
                addChild(enemy_sprite_);

                //collision_ = create_collision_circul( enemy_sprite_ );

                x_move_state_ = x_move_state::NONE;
                y_move_state_ = y_move_state::DOWN;
                move_speed_ = 1.f;
                return true;
            }
            //アップデート関数
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


        class enemy2 : base_enemy
        {
            //    //画面サイズの取得
            //    cocos2d::Size winSize = cocos2d::Director::sharedDirector()->getWinSize();

        public:
            ~enemy2()
            {
                enemy_sprite_->release();
            }
            CREATE_FUNC( enemy2 );

        public:
            //初期化関数
            virtual bool init() override
            {
                using namespace cocos2d;

                if( !Node::init() )
                {
                    return false;
                }
                struct once_init
                {
                    once_init()
                    {
                        add_sprite_batch( get_current_scene(), "img/player_bugdroid.png", "enemy" );
                    }
                } static once_;

                scheduleUpdate();
                setName( "enemy2" );

                enemy_sprite_ = create_sprite_from_batch( get_current_scene(), "enemy" );
                enemy_sprite_->retain();
                enemy_sprite_->setPosition( position_ );
                enemy_sprite_->setName( "enemy_sprite" );
                addChild( enemy_sprite_ );

                //collision_ = create_collision_circul( enemy_sprite_ );

                x_move_state_ = x_move_state::LEFT;
                y_move_state_ = y_move_state::DOWN;
                move_speed_ = 1.f;
                return true;
            }
            //アップデート関数
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