/************************************************************************************//**
    @file	c2xa/object/player.hpp
    @brief	player

    @author    新ゝ月(NewNotMoon)
    @date      2015/08/29
****************************************************************************************/
#ifndef C2XA_OBJECT_PLAYER_HPP
#define C2XA_OBJECT_PLAYER_HPP

#include <cocos2d.h>

#include <c2xa/bullet/test_player_bullet.hpp>

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
            bool touch_;
            bool tap_;
            float touch_count_;
            cocos2d::Point touch_position_;

        public:
            CREATE_FUNC( player );
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

                auto dispatcher = Director::getInstance()->getEventDispatcher();
                auto listener = EventListenerKeyboard::create();

                listener->onKeyPressed = [ & ]( EventKeyboard::KeyCode key_, Event* event_ )
                {
                    switch( key_ )
                    {
                    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                        move_state_ = move_state::LEFT; break;
                    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                        move_state_ = move_state::RIGHT; break;
                    case EventKeyboard::KeyCode::KEY_UP_ARROW:
                        getParent()->addChild( bullet::test_player_bullet::create( position_ ) ); break;
                    case EventKeyboard::KeyCode::KEY_ESCAPE:
                        this->removeFromParent(); break; // 自分を消す(アクションも削除されるので注意)
                    }
                };

                listener->onKeyReleased = [ & ]( EventKeyboard::KeyCode key_, Event* event_ )
                {
                    if( ( key_ == EventKeyboard::KeyCode::KEY_LEFT_ARROW && move_state_ == move_state::LEFT )
                     || ( key_ == EventKeyboard::KeyCode::KEY_RIGHT_ARROW && move_state_ == move_state::RIGHT ) )
                    {
                        move_state_ = move_state::NONE;
                    }
                };

                dispatcher->addEventListenerWithSceneGraphPriority( listener, this );

                auto touch_listener_ = EventListenerTouchOneByOne::create();

                touch_listener_->onTouchBegan = [ & ]( Touch* t_, Event* )
                {
                    touch_count_ = 0.f;
                    touch_ = true;
                    tap_ = true;
                    touch_position_ = t_->getLocation();
                    move_state_ = position_ > touch_position_.x ? move_state::LEFT : move_state::RIGHT;
                    return true;
                };

                touch_listener_->onTouchMoved = [ & ]( Touch* t_, Event* )
                {
                    touch_position_ = t_->getLocation();
                    move_state_ = position_ > touch_position_.x ? move_state::LEFT : move_state::RIGHT;
                };

                touch_listener_->onTouchCancelled = [ & ]( Touch* t_, Event* )
                {
                    touch_count_ = 0.f;
                    touch_ = false;
                    tap_ = false;
                    move_state_ = move_state::NONE;
                };

                touch_listener_->onTouchEnded = [ & ]( Touch* t_, Event* )
                {
                    if( tap_ )
                    {
                        getParent()->addChild( bullet::test_player_bullet::create( position_ ) );
                    }
                    touch_count_ = 0.f;
                    touch_ = false;
                    tap_ = false;
                    move_state_ = move_state::NONE;
                };

                this->getEventDispatcher()->addEventListenerWithSceneGraphPriority( touch_listener_, this );


                return true;
            }
            virtual void update( float delta_ ) override
            {
                if( touch_ )
                {
                    touch_count_ += delta_ * 100;
                    if( touch_count_ > 10.f )
                    {
                        tap_ = false;
                    }
                    if( touch_count_ > 5.f )
                    {
                        switch( move_state_ )
                        {
                        case move_state::LEFT:
                            {
                                position_ -= delta_ * 100; i_->setPositionX( position_ );
                                if( touch_position_.x <= position_ )
                                {
                                    i_->setPositionX( touch_position_.x );
                                    touch_count_ = 0;
                                    touch_ = false;
                                    tap_ = false;
                                }
                            }
                            break;
                        case move_state::RIGHT:
                            {
                                position_ += delta_ * 100; i_->setPositionX( position_ );
                                if( touch_position_.x >= position_ )
                                {
                                    i_->setPositionX( touch_position_.x );
                                    touch_count_ = 0;
                                    touch_ = false;
                                    tap_ = false;
                                }
                            }
                            break;
                        }
                    }
                }
            }
        };
    }
}

#endif//C2XA_OBJECT_PLAYER_HPP