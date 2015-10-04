
#include <c2xa/judgement.hpp>
#include <c2xa/scene/main_scene.hpp>
#include <c2xa/object/player.hpp>
#include <c2xa/object/coin.hpp>
#include <c2xa/utility.hpp>

using namespace c2xa;

bool judgement::init()
{
    using namespace cocos2d;

    if( !Node::init() )
    {
        return false;
    }

    scheduleUpdate();
    setName( "judgement" );

    auto keyboard_listener_ = EventListenerKeyboard::create();
    keyboard_listener_->onKeyPressed = [ & ]( EventKeyboard::KeyCode key_, Event* event_ )
    {
        if( key_ == EventKeyboard::KeyCode::KEY_ESCAPE )
        {
            game_over_ = true;
            get_parent<scene::main_scene>( this )->game_over();
        }
    };
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority( keyboard_listener_, this );

    return true;
}

void judgement::update( float )
{
    if( game_over_ )
    {
    }
    else
    {
        // 接触判定
        auto root_         = get_parent( this );
        auto object_layer_ = get_child( root_, "object_layer" );
        auto player_       = get_child<object::player>( object_layer_, "player" );
        auto coins_        = get_child( object_layer_, "coins" )->getChildren();
        auto enemies_        = get_child( object_layer_, "enemies" )->getChildren();
        //auto player_bullets_ = get_child( object_layer_, "player_bullets" )->getChildren();
        //auto enemy_bullets_ = get_child( object_layer_, "enemy_bullets" )->getChildren();

        // enemy<->player_bullets
        //for( auto b : player_bullets_ )
        //{
        //    auto player_bullet_ = static_cast<bullet::player_bullet*>( b );
        //  
        //    for( auto e : enemies_ )
        //    {
        //        auto enemy_ = static_cast<object::enemy_interface*>( e );
        //        if( enemy_->get_collision()->judge( player_bullets_->get_collision().get() ) )
        //        {
        //            enemy_->collide( object_type::player_bullet );
        //            player_bullets_->collide( object_type::enemy );
        //        }
        //    }
        //}

        // player<->enemy
        for( auto i : enemies_ )
        {
            auto enemy_ = static_cast<object::enemy_interface*>( i );
            if( player_->get_collision()->judge( enemy_->get_collision().get() ) )
            {
                enemy_->collide( object_type::player );
                player_->collide( object_type::enemy );
                //TODO: 自機減らす&僅かな判定無効時間
            }
        }

        // player<->enemy_bullets
        //for( auto i : enemy_bullets_ )
        //{
        //    auto enemy_bullet_ = static_cast<bullet::enemy_bullet_interface*>( i );
        //    if( player_->get_collision()->judge( enemy_bullet_->get_collision().get() ) )
        //    {
        //        enemy_bullet_->collide( object_type::player );
        //        player_->collide( object_type::enemy_bullet );
        //        //TODO: 自機減らす&僅かな判定無効時間
        //    }
        //}

        // player<->coin
        for( auto i : coins_ )
        {
            auto coin_ = static_cast<object::coin_interface*>( i );
            if( player_->get_collision()->judge( coin_->get_collision().get() ) )
            {
                score_.add( coin_->get_point() );
                coin_->collide( object_type::player );
                player_->collide( object_type::coin );
            }
        }

        // スコアカウント
        // コリジョンより後ろ
        get_child<scene::main::layer::ui_layer>( get_current_scene(), "ui_layer" )->set_score( score_.get() );
        // 自機数カウント
        // ゲーム終了判断
        if( remaining_.is_dead() )
        {
            game_over_ = true;
            get_parent<scene::main_scene>( this )->game_over();
        }
    }
}