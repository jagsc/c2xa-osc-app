/************************************************************************************//**
    @file    c2xa/object/super_coin.hpp
    @author  新ゝ月(NewNotMoon)
    @date    created on 2015/09/22
****************************************************************************************/
#ifndef C2XA_OBJECT_SUPER_COIN_HPP
#define C2XA_OBJECT_SUPER_COIN_HPP

#include <cocos2d.h>

#include <AudioEngine.h>

#include <c2xa/c2xa_config.hpp>
#include <c2xa/collision.hpp>
#include <c2xa/utility.hpp>
#include <c2xa/object/coin.hpp>

namespace c2xa
{
    namespace object
    {
        class super_coin
            : public coin_interface
        {
        private:
            cocos2d::Sprite* sprite_;
            collision collision_;
            float fall_speed_ = 5.f;

            cocos2d::Vec2 begin_;
            cocos2d::Vec2 end_;
            float duration_;
            float count_ = 0.f;

        public:
            ~super_coin()
            {
                sprite_->release();
            }

            CREATE_FUNC( super_coin );
            virtual bool init() override
            {
                using namespace cocos2d;

                if( !Node::init() )
                {
                    return false;
                }

                scheduleUpdate();

                sprite_ = create_sprite_from_batch( get_current_scene(), "CloseNormal.png" );
                sprite_->retain();

                static std::mt19937 engine_( random_seed );
                std::uniform_real_distribution<float> dist_( 0, app_width );

                auto x =  dist_( engine_ );
                sprite_->setPosition( x, app_height );
                sprite_->setScale( 2.f );

                addChild( sprite_ );

                collision_ = create_collision_circul( sprite_ );

                begin_ = Vec2{ dist_( engine_ ), app_height };
                end_ = Vec2{ dist_( engine_ ), 0 };
                duration_ = begin_.getDistance( end_ ) / fall_speed_;

                return true;
            }
            virtual void update( float delta_ ) override
            {
                count_ += get_delta();
                if( count_ > duration_ )
                {
                    removeFromParent();
                }
                else
                {
                    sprite_->setPosition( ( end_ - begin_ ) * count_ / duration_ + begin_ );
                }
            }
            unsigned int get_point() const override
            {
                return super_coin_point;
            }
            collision get_collision() const override
            {
                return collision_;
            }
            void collide( object_type ) override
            {
                cocos2d::experimental::AudioProfile p;
                p.maxInstances = 0; // 0は制限なしっぽい
                p.minDelay = 0; // 不明
                p.name = "get_coin"; // プロファイルに名前をつけられるっぽい？たぶんあとからプロファイルを取得したりする時に使う
                                     // play2d( ファイルパス, ループフラグ, 音量, プロファイルへのポインタ(nullptrでデフォルト) )
                cocos2d::experimental::AudioEngine::play2d( "sounds/get_coin.mp3", false, 0.3f, &p );
                removeFromParent();
            }
        };
    }
}

#endif//C2XA_OBJECT_SUPER_COIN_HPP