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
        class enemy : public cocos2d::Node {
        public :
            cocos2d::Vec2 position_ = { app_width/2, app_height*0.75 };//画面上中央
            int enemy_type_ = 1;

        private:
            cocos2d::Sprite *enemy_sprite_;
            float time = 0.f;
            cocos2d::Vec2 direction_ = { 0.f,0.f };
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

            collision collision_;

            //画面サイズの取得
            cocos2d::Size winSize = cocos2d::Director::sharedDirector()->getWinSize();

        public:
            CREATE_FUNC(enemy);

        public:
            //初期化関数
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

                collision_ = create_collision_circul( enemy_sprite_ );

                x_move_state_ = x_move_state::LEFT;
                y_move_state_ = y_move_state::DOWN;
                return true;
            }
            //アップデート関数
            virtual void update(float delta_) override
            {
                if (position_.y < -50.f) {
                    delete_enemy_node();
                }
                else {
                    direction(2);    //前進
                    position_ = move(delta_ * 100, direction_);
                    enemy_sprite_->setPosition(position_);
                    
                    time += (delta_ * 100);
                    if (time > 100) {
                        fire();
                        time = 0.f;
                    }
                }
            }

            collision get_collision() const
            {
                return collision_;
            }
        private:
            //弾発射
            void fire();
            //移動
            cocos2d::Vec2 move(float speed, cocos2d::Vec2 direction);
            void direction(int e);
            //破壊
            void delete_enemy_node();
        };
    }
}

#endif//C2XA_OBJECT_ENEMY_HPP