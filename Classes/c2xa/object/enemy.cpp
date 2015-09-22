#include <c2xa/object/enemy.hpp>
#include <c2xa/object/player.hpp>

using namespace c2xa::object;

void enemy::fire()
{

}

cocos2d::Vec2 enemy::move( float speed, cocos2d::Vec2 direction )
{
    return position_ + ( direction * speed );
}

void enemy::direction( int e )
{
    switch( e )
    {
    case 1: //前進
        switch( x_move_state_ )
        {
        case x_move_state::LEFT:
            direction_.x = 0.f;
            break;
        case x_move_state::RIGHT:
            direction_.x = 0.f;
            break;
        default:
            break;
        }
        switch( y_move_state_ )
        {
        case y_move_state::UP:
            direction_.y = 0.f;
            break;
        case y_move_state::DOWN:
            direction_.y = -1.f;
            break;
        default:
            break;
        }
        break;
    case 2: //ジグザグ
        switch( x_move_state_ )
        {
        case x_move_state::LEFT:
            direction_.x = -1.f;
            break;
        case x_move_state::RIGHT:
            direction_.x = 1.f;
            break;
        default:
            break;
        }
        switch( y_move_state_ )
        {
        case y_move_state::UP:
            direction_.y = 0.f;
            break;
        case y_move_state::DOWN:
            direction_.y = -1.f;
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
        break;
    default:
        break;
    }
}

void enemy::delete_enemy_node()
{
    removeFromParent();
}