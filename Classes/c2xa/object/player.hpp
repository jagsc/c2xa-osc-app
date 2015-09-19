/************************************************************************************//**
    @file	c2xa/object/player.hpp
    @brief	player

    @author    新ゝ月(NewNotMoon)
    @date      2015/08/29
****************************************************************************************/
#ifndef C2XA_OBJECT_PLAYER_HPP
#define C2XA_OBJECT_PLAYER_HPP

#include <cocos2d.h>

namespace c2xa
{
    namespace object
    {
        /*!
            @class player
            @author 新ゝ月(NewNotMoon)
            
            自機オブジェクトです。
            オブジェクトは全てCocos2d-xのNodeとして実装します。
            Nodeとしてのオーバーライド以外に外部インターフェイスは持っていません。
        */
        class player
            : public cocos2d::Node
        {
        public:
            static const int y_position = 100;

        private:
            float   position_;
            enum class move_state
            {
                NONE,
                LEFT,
                RIGHT
            } move_state_;
            float input_count_;
            bool  is_touch_;
            cocos2d::Point touch_position_;

        public:
            CREATE_FUNC( player );
            
        public:
            /*!
                @fn init

                Nodeからのオーバーライドです。
            */
            virtual bool init() override
            {
                using namespace cocos2d;

                if( !Node::init() )
                {
                    return false;
                }
                this->scheduleUpdate();


                position_ = y_position;
                auto player_sprite_ = Sprite::create( "bugdroid-player.png" );
                player_sprite_->setName( "player_sprite" );
                player_sprite_->setPosition( Vec2( position_, y_position ) );
                addChild( player_sprite_ );

                auto keyboard_listener_ = EventListenerKeyboard::create();
                auto touch_listener_    = EventListenerTouchOneByOne::create();

                keyboard_listener_->onKeyPressed = [ & ]( EventKeyboard::KeyCode key_, Event* event_ )
                {
                    switch( key_ )
                    {
                    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                        move_state_ = move_state::LEFT; break;
                    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                        move_state_ = move_state::RIGHT; break;
                    }
                    if( key_ == EventKeyboard::KeyCode::KEY_UP_ARROW && input_count_ < 5.f )
                    {
                        fire();
                    }
                };

                keyboard_listener_->onKeyReleased = [ & ]( EventKeyboard::KeyCode key_, Event* event_ )
                {
                    if( ( key_ == EventKeyboard::KeyCode::KEY_LEFT_ARROW && move_state_ == move_state::LEFT )
                     || ( key_ == EventKeyboard::KeyCode::KEY_RIGHT_ARROW && move_state_ == move_state::RIGHT ) )
                    {
                        reset();
                    }
                };

                touch_listener_->onTouchBegan = [ & ]( Touch* t_, Event* )
                {
                    if( move_state_ != move_state::NONE )
                    {
                        return false;
                    }
                    is_touch_ = true;
                    input_count_ = 0.f;
                    touch_position_ = t_->getLocation();
                    move_state_ = position_ > touch_position_.x ? move_state::LEFT : move_state::RIGHT;
                    return true;
                };

                touch_listener_->onTouchMoved = [ & ]( Touch* t_, Event* )
                {
                    is_touch_ = true;
                    touch_position_ = t_->getLocation();
                    move_state_ = position_ > touch_position_.x ? move_state::LEFT : move_state::RIGHT;
                };

                touch_listener_->onTouchCancelled = [ & ]( Touch* t_, Event* )
                {
                    reset();
                };

                touch_listener_->onTouchEnded = [ & ]( Touch* t_, Event* )
                {
                    if( input_count_ <= 10.f )
                    {
                        fire();
                    }
                    reset();
                };

                auto dispatcher = Director::getInstance()->getEventDispatcher();
                dispatcher->addEventListenerWithSceneGraphPriority( keyboard_listener_, this );
                dispatcher->addEventListenerWithSceneGraphPriority( touch_listener_, this );

                return true;
            }
            /*!
                @fn update
                @param[in] delta_ 各呼び出し時におけるフレームの遅れ

                Nodeからのオーバーライドです。直接呼び出しません。
            */
            virtual void update( float delta_ ) override
            {
                if( move_state_ != move_state::NONE )
                {
                    input_count_ += delta_ * 100;
                    auto player_sprite_ = getChildByName( "player_sprite" );
                    switch( move_state_ )
                    {
                    case move_state::LEFT:
                    {
                        if( input_count_ > 5.f )
                        {
                            position_ -= delta_ * 100; player_sprite_->setPositionX( position_ );
                        }
                        if( is_touch_ && touch_position_.x >= position_ )
                        {
                            player_sprite_->setPositionX( touch_position_.x );
                            reset();
                        }
                    }
                    break;
                    case move_state::RIGHT:
                    {
                        if( input_count_ > 5.f )
                        {
                            position_ += delta_ * 100; player_sprite_->setPositionX( position_ );
                        }
                        if( is_touch_ && touch_position_.x <= position_ )
                        {
                            player_sprite_->setPositionX( touch_position_.x );
                            reset();
                        }
                    }
                    break;
                    }
                }
            }

        private:
            /*!
                @fn reset

                入力関係のメンバをリセットします。
            */
            void reset()
            {
                is_touch_ = false;
                input_count_ = 0.f;
                move_state_ = move_state::NONE;
            };
            /*!
                @fn fire

                自機から弾を発射します。
            */
            void fire();
        };
    }
}

#endif//C2XA_OBJECT_PLAYER_HPP