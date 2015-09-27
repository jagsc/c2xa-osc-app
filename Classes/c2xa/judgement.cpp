
#include <c2xa/judgement.hpp>
#include <c2xa/scene/main_scene.hpp>
#include <c2xa/object/player.hpp>
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
        //auto enemies_      = get_child( object_layer_, "enemies" )->getChildren();

        //for( auto i : enemies_ )
        //{
        //    auto enemy_ = static_cast<object::enemy_interface*>( i );
        //    if( player_->get_collision()->judge( enemy_->get_collision().get() ) )
        //    {
        //        score_.add( enemy_->get_point() );
        //        enemy_->on_collide();
        //    };
        //}
        for( auto i : coins_ )
        {
            auto coin_ = static_cast<object::coin_interface*>( i );
            if( player_->get_collision()->judge( coin_->get_collision().get() ) )
            {
                score_.add( coin_->get_point() );
                coin_->on_collide();
            };
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