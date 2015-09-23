#include <c2xa/object/enemy.hpp>
#include <c2xa/object/player.hpp>

using namespace c2xa::object;


void enemy_interface::delete_enemy_node()
{
    removeFromParent();
}

void enemy1::move()
{
    switch( x_move_state_ )
    {
    case x_move_state::LEFT:
        move_direction_.x = 0.f;
        break;
    case x_move_state::RIGHT:
        move_direction_.x = 0.f;
        break;
    default:
        break;
    }
    switch( y_move_state_ )
    {
    case y_move_state::UP:
        move_direction_.y = 0.f;
        break;
    case y_move_state::DOWN:
        move_direction_.y = -1.f;
        break;
    default:
        break;
    }
    position_ = position_ + ( move_direction_ * move_speed_ );
    enemy_sprite_->setPosition( position_ );
}

void enemy1::fire()
{
    //エネミーの弾の軌道
}

void enemy2::move()
{
    switch( x_move_state_ )
    {
    case x_move_state::LEFT:
        move_direction_.x = -1.f;
        break;
    case x_move_state::RIGHT:
        move_direction_.x = 1.f;
        break;
    default:
        break;
    }
    switch( y_move_state_ )
    {
    case y_move_state::UP:
        move_direction_.y = 0.f;
        break;
    case y_move_state::DOWN:
        move_direction_.y = -1.f;
        break;
    default:
        break;
    }
    if( position_.x > app_width - 100 )
    {
        x_move_state_ = x_move_state::LEFT;
    }
    else if( position_.x < 100 )
    {
        x_move_state_ = x_move_state::RIGHT;
    }
    position_ = position_ + ( move_direction_ * move_speed_ );
    enemy_sprite_->setPosition( position_ );
}

void enemy2::fire()
{

}