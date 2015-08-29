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
            bool direction_;
            int touch_count_;
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
                    touch_count_ = 1;
                    touch_ = true;
                    tap_ = true;
                    touch_position_ = t_->getLocation();
                    if( position_ < touch_position_.x )
                    {
                        direction_ = true;
                    }
                    else
                    {
                        direction_ = false;
                    }
                    return true;
                };

                touch_listener_->onTouchMoved = [ & ]( Touch* t_, Event* )
                {
                    touch_position_ = t_->getLocation();
                    if( position_ < touch_position_.x )
                    {
                        direction_ = true;
                    }
                    else
                    {
                        direction_ = false;
                    }
                };

                touch_listener_->onTouchCancelled = [ & ]( Touch* t_, Event* )
                {
                    touch_count_ = 0;
                    touch_ = false;
                    tap_ = false;
                };

                touch_listener_->onTouchEnded = [ & ]( Touch* t_, Event* )
                {
                    if( tap_ )
                    {
                        getParent()->addChild( bullet::test_player_bullet::create( position_ ) );
                    }
                    touch_count_ = 0;
                    touch_ = false;
                    tap_ = false;
                };

                this->getEventDispatcher()->addEventListenerWithSceneGraphPriority( touch_listener_, this );


                return true;
            }
            virtual void update( float delta_ ) override
            {
                switch( move_state_ )
                {
                case move_state::LEFT:
                    position_ -= 1.f * ( delta_ * 100 ); i_->setPositionX( position_ ); break;
                case move_state::RIGHT:
                    position_ += 1.f * ( delta_ * 100 ); i_->setPositionX( position_ ); break;
                }
                if( touch_ )
                {
                    ++touch_count_;
                    if( touch_count_ > 10 )
                    {
                        tap_ = false;
                    }
                    if( !tap_ )
                    {
                        if( direction_ )
                        {
                            position_ += 1.f * ( delta_ * 100 ); i_->setPositionX( position_ );
                            if( touch_position_.x <= position_ )
                            {
                                i_->setPositionX( touch_position_.x );
                                touch_count_ = 0;
                                touch_ = false;
                                tap_ = false;
                            }
                        }
                        else
                        {
                            position_ -= 1.f * ( delta_ * 100 ); i_->setPositionX( position_ );
                            if( touch_position_.x >= position_ )
                            {
                                i_->setPositionX( touch_position_.x );
                                touch_count_ = 0;
                                touch_ = false;
                                tap_ = false;
                            }
                        }
                    }
                }
            }
        };
    }
}

#endif//C2XA_OBJECT_PLAYER_HPP