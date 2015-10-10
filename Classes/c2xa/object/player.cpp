/************************************************************************************//**
    @file   c2xa/object/player.cpp
    @author êVÅTåé(NewNotMoon)
    @date   2015/08/29
****************************************************************************************/

#include <c2xa/object/player.hpp>
#include <c2xa/bullet/player_bullet.hpp>
#include <c2xa/c2xa_config.hpp>
#include <c2xa/utility.hpp>

using namespace c2xa::object;
using namespace cocos2d;

namespace
{
    static constexpr float move_speed = 6.f;
    static constexpr float move_count = 10.f;
    static constexpr float fire_count = 15.f;
    static constexpr float max_rotation_degree = 45.f;
}

bool player::init()
{
    if( !Node::init() )
    {
        return false;
    }
    scheduleUpdate();
    setName( "player" );

    position_ = player_y_position;
    auto player_sprite_ = Sprite::create( "img/player_bugdroid.png" );
    player_sprite_->setName( "player_sprite" );
    player_sprite_->setPosition( Vec2( position_, player_y_position ) );
    addChild( player_sprite_ );

    collision_ = create_collision_circul( player_sprite_ );

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
        if( key_ == EventKeyboard::KeyCode::KEY_UP_ARROW )
        {
            reset();
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
        if( input_count_ <= fire_count )
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

void player::update( float delta_ )
{
    auto player_sprite_ = getChildByName( "player_sprite" );
    float target_rotation_ = 0.f;
    if( move_state_ != move_state::NONE )
    {
        if( input_count_ > 10.f )
        {
            if( is_touch_ )
            {
                auto distance_ = position_ - touch_position_.x;
                if( std::abs( distance_ ) > 100.f )
                {
                    target_rotation_ = move_state_ == move_state::LEFT ? -max_rotation_degree : max_rotation_degree;
                }
                else
                {
                    target_rotation_ = -( max_rotation_degree * distance_ / 100.f );
                }
            }
            else
            {
                if( input_count_ < 60.f )
                {
                    target_rotation_ = ( max_rotation_degree * ( input_count_ - 10.f ) / 50.f );
                    target_rotation_ = move_state_ == move_state::LEFT ? -target_rotation_ : target_rotation_;
                }
                else
                {
                    target_rotation_ = move_state_ == move_state::LEFT ? -max_rotation_degree : max_rotation_degree;
                }
            }
        }
        input_count_ += delta_ * 100;
        if( input_count_ > move_count )
        {
            switch( move_state_ )
            {
            case move_state::LEFT:
            {
                position_ -= move_speed * delta_ * 100;
                if( is_touch_ && touch_position_.x >= position_ )
                {
                    position_ = touch_position_.x;
                }
                if( position_ <= 0.f )
                {
                    position_ = 0.f;
                }
            }
            break;
            case move_state::RIGHT:
            {
                position_ += move_speed * delta_ * 100;
                if( is_touch_ && touch_position_.x <= position_ )
                {
                    position_ = touch_position_.x;
                }
                if( position_ >= app_width )
                {
                    position_ = app_width;
                }
            }
            break;
            }
            player_sprite_->setPositionX( position_ );
        }
    }
    else
    {
        target_rotation_ = player_sprite_->getRotation() * ( 1.f + std::abs( player_sprite_->getRotation() ) ) / 47.f;
    }
    player_sprite_->setRotation( ( target_rotation_ + player_sprite_->getRotation() ) / 2.f );
}

void player::reset()
{
    is_touch_ = false;
    input_count_ = 0.f;
    move_state_ = move_state::NONE;
}

void player::fire()
{
    getParent()->getChildByName("player_bullets")->addChild( bullet::player_bullet::create( position_ ) );
}