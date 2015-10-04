#include <c2xa/object/enemy.hpp>
#include <c2xa/object/player.hpp>
//
//using namespace c2xa::object;
//
//
//void base_enemy::delete_enemy_node()
//{
//    removeFromParent();
//}
//
//void enemy1::move(float time)
//{
//    move_direction_.x =0.f;
//    move_direction_.y = -1.f;
//    position_ = position_ + ( move_direction_ * move_speed_ );
//    enemy_sprite_->setPosition( position_ );
//}
//
//void enemy1::fire( float time )
//{
//    //エネミーの弾の軌道
//}
//
//void enemy2::move( float time )
//{
//    if( dir == 0 )
//    {
//        if( position_.x > app_height )
//        {
//            delete_enemy_node();
//        }else{
//            move_direction_.x = -1.f;
//            move_direction_.y = 0.f;
//        }
//    }
//    else
//    {
//        if( position_.x < 0 )
//        {
//            delete_enemy_node();
//        }
//        else
//        {
//            move_direction_.x = 1.f;
//            move_direction_.y = 0.f;
//        }
//    }
//    position_ = position_ + ( move_direction_ * move_speed_ );
//    enemy_sprite_->setPosition( position_ );
//}
//
//void enemy2::fire( float time )
//{
//
//}
//
//void enemy3::move( float time )
//{
//    switch( state )
//    {
//    case STATE_TYPE::APPEAR:
//        move_speed_ = move_speed_ - time;
//        if( move_speed_ < 0.f )
//        {
//            move_speed_ = 10.0f;
//            int random = 1 + (int)( rand() *( 20 - 1 + 1.f ) / ( 1.f + RAND_MAX ) );
//            position_ ={ app_width / random ,app_height / random };
//            enemy_sprite_->setPosition( position_ );
//            state = STATE_TYPE::ACT;
//        }
//        break;
//    case STATE_TYPE::ACT:
//        fire(time );
//        move_speed_ = move_speed_ - time;
//        if( move_speed_ < 0.f )
//        {
//            state = STATE_TYPE::DISAPPEAR;
//        }
//        break;
//    case STATE_TYPE::DISAPPEAR:
//        delete_enemy_node();
//        break;
//    default:
//        break;
//    }
//}
//
//void enemy3::fire( float time )
//{
//
//}
//
//void enemy4::move( float time )
//{
//    if( index < num_orbit )
//    {
//        timer[ index ] -= time;
//        if( timer[ index ] > 0.f )
//        {
//            position_.x += vx[ index ] * move_speed_;
//            position_.y += vy[ index ] * move_speed_;
//            enemy_sprite_->setPosition( position_ );
//        }
//        else
//        {
//            index++;
//        }
//    }
//    else
//    {
//        delete[] vx;
//        delete[] vy;
//        delete[] timer;
//        delete_enemy_node();
//    }
//}
//
//void enemy4::fire( float time )
//{
//
//}