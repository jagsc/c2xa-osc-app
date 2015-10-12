#include <c2xa/object/enemy.hpp>
#include <c2xa/object/player.hpp>

using namespace c2xa::object;

/// 引数のLuaステートに関数を登録します
void enemy::registrory_glue( lua_State* state_ )
{
    static const struct luaL_Reg functions_[] ={
        { "create_enemy", create_glue },
        { "set_magnification", set_magnification_glue },
        { "get_magnification", get_magnification_glue },
        { nullptr, nullptr }
    };
    luaL_register( state_, "c2xa", functions_ );
}
/// この関数はLuaに登録されます
/// @attention Luaから呼び出す関数です
///
/// この関数はLuaスタックトップに1つの値が積まれていることを期待します
/// その引数はテーブル型であり、適切なフィールドを持っていると期待します
int enemy::create_glue( lua_State* state_ )
{
    std::unique_ptr<data> data_{ new data }; // make_uniqueはC++11にないので我慢…

    data_->state = state_;

    //lua_getfield( state_, -1, "scheduler_id" );
    //CCASSERT( lua_isnumber( state_, -1 ), "" );
    //data_->scheduler_id = lua_tointeger( state_, -1 );

    //lua_getfield( state_, -1, "move" );
    //CCASSERT( lua_type( state_, -1 ) == LUA_TFUNCTION, "" );
    //data_->move_id = luaL_ref( state_, LUA_REGISTRYINDEX );

    data_->update_function = lua::to< lua::type::function >::from_table( state_, "update" );
    data_->duration = lua::to< lua::type::number >::from_table( state_, "duration" );
    data_->point = lua::to< lua::type::number >::from_table( state_, "point" );

    lua_getfield( state_, -1, "start" );
    data_->start_x = lua::to< lua::type::number >::from_table( state_, "x" );
    data_->start_y = lua::to< lua::type::number >::from_table( state_, "y" );
    lua_settop( data_->state, -2 );

    lua_getfield( state_, -1, "goal" );
    data_->goal_x = lua::to< lua::type::number >::from_table( state_, "x" );
    data_->goal_y = lua::to< lua::type::number >::from_table( state_, "y" );
    lua_settop( data_->state, -2 );

    lua_getfield( state_, -1, "fire" );
    if( lua_type( state_, -1 ) == LUA_TTABLE )
    {
        lua_pushnil( state_ ); //たぶんNilだと最初からって意味
        while( lua_next( state_, -2 ) != 0 )
        {
            if( lua_type( state_, -1 ) == LUA_TTABLE )
            {
                // テーブルの場合は、弾だと判断します
                data::fire::info::bullet b_;
                b_.update_function = lua::to< lua::type::function >::from_table( state_, "update" );
                b_.duration = lua::to< lua::type::number >::from_table( state_, "duration" );
                lua_getfield( state_, -1, "goal" );
                b_.goal_x = lua::to< lua::type::number >::from_table( state_, "x" );
                b_.goal_y = lua::to< lua::type::number >::from_table( state_, "y" );
                lua_settop( data_->state, -2 );
                data_->fire_info_.push_back( { data::fire::type::bullet, b_ } );
            }
            else if( lua_type( state_, -1 ) == LUA_TNUMBER )
            {
                // 数値の場合は、待機時間だと判断します
                auto time_ = lua_tonumber( state_, -1 );
                data_->fire_info_.push_back( { data::fire::type::time, data::fire::info{ time_ } } );
            }
            else
            {
                CCASSERT( false, "値でもテーブルでもねえぞｵｲ！" );
            }
            lua_settop( state_, -2 );
        }
    }
    lua_settop( state_, 0 );

    auto enemy_ = create_template<enemy>( std::move( data_ ) );

    lua::push_usertype( state_, enemy_, "cc.Node" );

    return 1; // Luaの受け取る戻り値の数は1(スタックトップの1つ目が戻り値として認識される)
}
bool enemy::init( std::unique_ptr<data>&& d_ ) // no-override
{
    if( !Node::init() )
    {
        return false;
    }
    data_ = std::move( d_ );
    scheduleUpdate();

    auto sprite_ = create_sprite_from_batch( get_current_scene(), "img/enemy_bugdroid.png" );
    sprite_->setPosition( get_position() );
    sprite_->setName( "sprite" );
    addChild( sprite_ );

    collision_ = create_collision_circul( sprite_ );

    fire();

    return true;
}
void enemy::update( float delta_ )
{
    progress_ += get_count();

    if( progress_ > data_->duration * 60 )
    {
        clean();
        return;
    }

    get_child<cocos2d::Sprite>( this, "sprite" )->setPosition( get_position() );
    fire();
}
void enemy::clean()
{
    unscheduleUpdate();
    luaL_unref( data_->state, LUA_REGISTRYINDEX, data_->update_function );
    for( auto i : data_->fire_info_ )
    {
        if( i.type_ == data::fire::type::bullet )
        {
            luaL_unref( data_->state, LUA_REGISTRYINDEX, i.info_.bullet_.update_function );
        }
    }
    removeFromParent();
}
void enemy::fire()
{
    if( !data_->fire_info_.empty() )
    {
        auto info_ = data_->fire_info_.front();
        while( info_.type_ != data::fire::type::bullet )
        {
            data_->fire_info_.pop_front();
            previous_fire_time_ += info_.info_.time_ * 60;
            if( data_->fire_info_.empty() )
            {
                break;
            }
            info_ = data_->fire_info_.front();
        }
        if( progress_ > previous_fire_time_ )
        {
            while( info_.type_ != data::fire::type::time )
            {
                std::unique_ptr<bullet::data> d_{ new bullet::data };
                d_->state = data_->state;
                d_->update_function = info_.info_.bullet_.update_function;
                d_->duration = info_.info_.bullet_.duration;
                auto pos_ = get_child<cocos2d::Sprite>( this, "sprite" )->getPosition();
                d_->start_x = pos_.x;
                d_->start_y = pos_.y;
                d_->goal_x = info_.info_.bullet_.goal_x;
                d_->goal_y = info_.info_.bullet_.goal_y;
                auto bullet_ = bullet::enemy_bullet::create( std::move( d_ ) );
                get_child( getParent()->getParent(), "enemy_bullets" )->addChild( bullet_ );
                data_->fire_info_.pop_front();
                if( data_->fire_info_.empty() )
                {
                    break;
                }
                info_ = data_->fire_info_.front();
            }
            previous_fire_time_ = progress_;
        }
    }
}
cocos2d::Vec2 enemy::get_position() const
{
    // 呼び出す関数: move_idの参照先
    lua_rawgeti( data_->state, LUA_REGISTRYINDEX, data_->update_function );

    // 第一引数: 始点(xとyを持つテーブル)
    lua_createtable( data_->state, 0, 2 );
    lua_pushnumber( data_->state, data_->start_x );
    lua_setfield( data_->state, -2, "x" );
    lua_pushnumber( data_->state, data_->start_y );
    lua_setfield( data_->state, -2, "y" );

    // 第一引数: 終点(xとyを持つテーブル)
    lua_createtable( data_->state, 0, 2 );
    lua_pushnumber( data_->state, data_->goal_x );
    lua_setfield( data_->state, -2, "x" );
    lua_pushnumber( data_->state, data_->goal_y );
    lua_setfield( data_->state, -2, "y" );

    // 第三引数: 進捗率(0～100までのdouble)
    lua_pushnumber( data_->state, progress_ * 100 / ( data_->duration * 60 ) );

    // 呼び出し: 引数3: 戻り値1: テーブル
    lua::call( data_->state, 3, 1 );

    lua_getfield( data_->state, -1, "position" );
    double x = lua::to< lua::type::number >::from_table( data_->state, "x" );
    double y = lua::to< lua::type::number >::from_table( data_->state, "y" );
    lua_settop( data_->state, -2 );

    return std::move( cocos2d::Vec2{
        static_cast<float>( x ),
        static_cast<float>( y )
    } );
}
unsigned int enemy::get_point() const
{
    return data_->point;
}
c2xa::collision enemy::get_collision() const
{
    return collision_;
}
void enemy::collide( c2xa::object_type type_ )
{
    // 自機の弾か自機本体
    // ここもLuaから指定できるようにする？とりあえずまずはC++で実装
    switch( type_ )
    {
    case object_type::player:
    {
        break;
    }
    case object_type::player_bullet:
    {
        if( !death_ )
        {
            death_ = true;
            auto effect_layer_ = getParent()->getParent();
            auto sprite_1_ = create_sprite_from_batch( effect_layer_, "img/main_scene_death_1.png" );
            sprite_1_->setPosition( get_position() );
            effect_layer_->addChild( sprite_1_ );
            sprite_1_->runAction(
                cocos2d::Sequence::create(
                    cocos2d::CallFunc::create( [ sprite_1_ ]
                    {
                        cocos2d::experimental::AudioEngine::play2d( "sounds/death.mp3", false, 0.3f, nullptr );
                    } ),
                    cocos2d::DelayTime::create( 0.3f ),
                    cocos2d::FadeOut::create( 0.7f ),
                    cocos2d::CallFunc::create( [ sprite_1_ ]
                    {
                        sprite_1_->removeFromParent();
                    } ),
                    nullptr
                ) );
            auto sprite_2_ = create_sprite_from_batch( effect_layer_, "img/main_scene_death_2.png" );
            sprite_2_->setPosition( get_position() );
            effect_layer_->addChild( sprite_2_ );
            sprite_2_->runAction(
                cocos2d::Sequence::create(
                    cocos2d::Spawn::createWithTwoActions(
                        cocos2d::RotateBy::create( 1.f, 5300.f ),
                        cocos2d::Sequence::create( cocos2d::DelayTime::create( 0.3f ), cocos2d::FadeOut::create( 0.5f ), nullptr )
                    ),
                    cocos2d::CallFunc::create( [ sprite_2_ ]
                    {
                        sprite_2_->removeFromParent();
                    } ),
                    nullptr
                ) );
            clean();
            break;
        }
    }
    }
}

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