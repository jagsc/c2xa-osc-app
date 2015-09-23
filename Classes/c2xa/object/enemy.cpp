#include <c2xa/object/enemy.hpp>
#include <c2xa/object/player.hpp>

using namespace c2xa::object;


void base_enemy::delete_enemy_node()
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
}

void enemy1::fire()
{
    //エネミーの弾の軌道
}

    //case 2: //ジグザグ
    //    switch( x_move_state_ )
    //    {
    //    case x_move_state::LEFT:
    //        direction_.x = -1.f;
    //        break;
    //    case x_move_state::RIGHT:
    //        direction_.x = 1.f;
    //        break;
    //    default:
    //        break;
    //    }
    //    switch( y_move_state_ )
    //    {
    //    case y_move_state::UP:
    //        direction_.y = 0.f;
    //        break;
    //    case y_move_state::DOWN:
    //        direction_.y = -1.f;
    //        break;
    //    default:
    //        break;
    //    }
    //    if( position_.x > app_width - 100 )
    //    {
    //        x_move_state_ = x_move_state::LEFT;
    //    }
    //    else if( position_.x < 100 )
    //    {
    //        x_move_state_ = x_move_state::RIGHT;
    //    }
    //    break;
    //default:
    //    break;
    //}
