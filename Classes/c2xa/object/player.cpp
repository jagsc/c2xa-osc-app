/************************************************************************************//**
    @file   c2xa/object/player.cpp
    @author 新ゝ月(NewNotMoon)
    @date   2015/08/29
****************************************************************************************/

#include <AudioEngine.h>

#include <c2xa/object/player.hpp>
#include <c2xa/bullet/player_bullet.hpp>
#include <c2xa/c2xa_config.hpp>
#include <c2xa/utility.hpp>

using namespace c2xa::object;
using namespace cocos2d;

namespace
{
    static constexpr float move_speed = 3.f;
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
    player_sprite_->setName( "sprite" );
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
    auto player_sprite_ = getChildByName( "sprite" );
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
        switch( move_state_ )
        {
        case move_state::LEFT:
        {
            if( input_count_ > move_count )
            {
                position_ -= move_speed * delta_ * 100;
                player_sprite_->setPositionX( position_ );
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
            if( input_count_ > move_count )
            {
                position_ += move_speed * delta_ * 100;
                player_sprite_->setPositionX( position_ );
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
    else
    {
        target_rotation_ = player_sprite_->getRotation() * ( 1.f + std::abs( player_sprite_->getRotation() ) ) / 47.f;
    }
    player_sprite_->setRotation( ( target_rotation_ + player_sprite_->getRotation() ) / 2.f );
}

void player::fire()
{
    getParent()->getChildByName("player_bullets")->addChild( bullet::player_bullet::create( position_ ) );
}

void player::collide( c2xa::object_type type_ )
{
    // 何かと接触しましたよー！！！？？？
    // 接触したのが敵なら自機のダメージエフェクトとか
    // 不要なら空
    switch( type_ )
    {
    case object_type::coin: break;
    case object_type::enemy:
    case object_type::enemy_bullet:
    {
        // ダメージ
        get_child( this, "sprite" )->runAction(
            cocos2d::Sequence::create(
                cocos2d::Blink::create( 3.f, 20 ),
                cocos2d::CallFunc::create( [ this ]
                {
                    get_child( this, "sprite" )->setVisible( true );
                }), nullptr ) );
        cocos2d::experimental::AudioEngine::play2d( "sounds/damage.mp3", false, 0.3f, nullptr );
        break;
    }
    default: break;
    }
}