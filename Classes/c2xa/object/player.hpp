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
        class player
            : public cocos2d::Node
        {
        public:
            static const int y_position = 100;

        private:
            cocos2d::Sprite* i_;
            float   position_;
            enum class move_state
            {
                NONE,
                LEFT,
                RIGHT
            } move_state_;
            float touch_count_;
            bool  is_touch_;
            cocos2d::Point touch_position_;

        public:
            CREATE_FUNC( player );
            
        public:
            virtual bool init() override
            {
                using namespace cocos2d;

                if( !Node::init() )
                {
                    return false;
                }
                this->scheduleUpdate();


                i_ = Sprite::create( "CloseNormal.png" );
                position_ = y_position;
                i_->setPosition( Vec2( position_, y_position ) );
                this->addChild( i_ );

                auto keyboard_listener = EventListenerKeyboard::create();
                auto touch_listener_   = EventListenerTouchOneByOne::create();

                keyboard_listener->onKeyPressed = [ & ]( EventKeyboard::KeyCode key_, Event* event_ )
                {
                    switch( key_ )
                    {
                    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                        move_state_ = move_state::LEFT; break;
                    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                        move_state_ = move_state::RIGHT; break;
                    case EventKeyboard::KeyCode::KEY_ESCAPE:
                        this->removeFromParent(); break; // 自分を消す(アクションも削除されるので注意)
                    }
                    if( key_ == EventKeyboard::KeyCode::KEY_UP_ARROW && touch_count_ < 5.f )
                    {
                        fire();
                    }
                };

                keyboard_listener->onKeyReleased = [ & ]( EventKeyboard::KeyCode key_, Event* event_ )
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
                    touch_count_ = 0.f;
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
                    if( touch_count_ <= 10.f )
                    {
                        fire();
                    }
                    reset();
                };

                auto dispatcher = Director::getInstance()->getEventDispatcher();
                dispatcher->addEventListenerWithSceneGraphPriority( keyboard_listener, this );
                dispatcher->addEventListenerWithSceneGraphPriority( touch_listener_, this );

                return true;
            }
            virtual void update( float delta_ ) override
            {
                if( move_state_ != move_state::NONE )
                {
                    touch_count_ += delta_ * 100;
                    switch( move_state_ )
                    {
                    case move_state::LEFT:
                    {
                        if( touch_count_ > 5.f )
                        {
                            position_ -= delta_ * 100; i_->setPositionX( position_ );
                        }
                        if( is_touch_ && touch_position_.x >= position_ )
                        {
                            i_->setPositionX( touch_position_.x );
                            reset();
                        }
                    }
                    break;
                    case move_state::RIGHT:
                    {
                        if( touch_count_ > 5.f )
                        {
                            position_ += delta_ * 100; i_->setPositionX( position_ );
                        }
                        if( is_touch_ && touch_position_.x <= position_ )
                        {
                            i_->setPositionX( touch_position_.x );
                            reset();
                        }
                    }
                    break;
                    }
                }
            }

        private:
            void reset()
            {
                is_touch_ = false;
                touch_count_ = 0.f;
                move_state_ = move_state::NONE;
            };
            void fire();

            void on_key_pressed( cocos2d::Touch* t_, cocos2d::Event* )
            {
            }
            void on_key_released( cocos2d::Touch* t_, cocos2d::Event* )
            {
            }
            void on_touch_began( cocos2d::Touch* t_, cocos2d::Event* )
            {
            }
            void on_touch_moved( cocos2d::Touch* t_, cocos2d::Event* )
            {
            }
            void on_touch_cancelled( cocos2d::Touch* t_, cocos2d::Event* )
            {
            }
            void on_touch_ended( cocos2d::Touch* t_, cocos2d::Event* )
            {
            }
        };
    }
}

#endif//C2XA_OBJECT_PLAYER_HPP