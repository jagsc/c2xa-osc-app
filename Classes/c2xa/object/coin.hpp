/************************************************************************************//**
    @file    c2xa/object/coin.hpp
    @author  新ゝ月(NewNotMoon)
    @date    created on 2015/09/20
****************************************************************************************/
#ifndef C2XA_OBJECT_COIN_HPP
#define C2XA_OBJECT_COIN_HPP

#include <cocos2d.h>

#include <AudioEngine.h>

#include <c2xa/c2xa_config.hpp>
#include <c2xa/utility.hpp>
#include <c2xa/object/object.hpp>


namespace c2xa
{
    namespace object
    {
        class coin_interface
            : public cocos2d::Node
            , public object_interface
        {
        };

        class coin
            : public coin_interface
        {
        private:
            cocos2d::Sprite* sprite_;
            collision collision_;
            float fall_speed_ = 1.f;

        public:
            ~coin()
            {
                sprite_->release();
            }

            CREATE_FUNC( coin );
            virtual bool init() override
            {
                using namespace cocos2d;

                if( !Node::init() )
                {
                    return false;
                }
                scheduleUpdate();

                sprite_ = create_sprite_from_batch( get_current_scene(), "img/main_scene_coin.png" );
                sprite_->retain();

                static std::mt19937 engine_( random_seed );
                std::uniform_real_distribution<> dist_( 0, app_width );
                auto x =  dist_( engine_ );
                sprite_->setPosition( x, app_height );

                addChild( sprite_ );

                collision_ = create_collision_circul( sprite_ );

                return true;
            }
            virtual void update( float delta_ ) override
            {
                auto y_ = sprite_->getPositionY() - ( fall_speed_ * get_count() );
                sprite_->setPositionY( y_ );
                if( y_ < 0 )
                {
                    removeFromParent();
                }
            }
            unsigned int get_point() const override
            {
                return coin_point;
            }
            collision get_collision() const override
            {
                return collision_;
            }
            void collide( object_type ) override
            {
                // コインはそもそも自機としか接触判定しないので分岐なし

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

#endif//C2XA_OBJECT_COIN_HPP