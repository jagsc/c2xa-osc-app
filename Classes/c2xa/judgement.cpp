
#include <c2xa/judgement.hpp>
#include <c2xa/scene/main_scene.hpp>
#include <c2xa/bullet/player_bullet.hpp>
#include <c2xa/object/player.hpp>
#include <c2xa/object/enemy.hpp>
#include <c2xa/object/coin.hpp>
#include <c2xa/utility.hpp>

using namespace c2xa;

bool judgement::init( cocos2d::Node* parent_ )
{
    using namespace cocos2d;

    if( !Node::init() )
    {
        return false;
    }

    scheduleUpdate();
    setName( "judgement" );

    auto ui_layer_ = get_child( parent_, "ui_layer" );
    auto remaining_node_ = Node::create();
    remaining_node_->setName( "remaining" );
    for( int i = remaining_.get(); 0 < i; --i )
    {
        auto sprite_ = create_sprite_from_batch( remaining_node_, "img/main_scene_remaining.png" );
        sprite_->setTag( i );
        sprite_->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
        sprite_->setPosition( Vec2{ 365.f + ( i - 1 ) * 35.f, app_height - 20.f } );
        remaining_node_->addChild( sprite_ );
    }
    ui_layer_->addChild( remaining_node_ );

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

void judgement::update( float delta_ )
{
    if( game_over_ )
    {
    }
    else
    {
        // 接触判定
        auto root_           = get_parent( this );
        auto object_layer_   = get_child( root_, "object_layer" );
        auto player_         = get_child<object::player>( object_layer_, "player" );
        auto coins_          = get_child( object_layer_, "coins" )->getChildren();
        auto enemies_        = get_child( object_layer_, "enemies" )->getChildren();
        auto player_bullets_ = get_child( object_layer_, "player_bullets" )->getChildren();
        auto enemy_bullets_  = get_child( object_layer_, "enemy_bullets" )->getChildren();

        // enemy<->player_bullets
        for( auto b : player_bullets_ )
        {
            auto player_bullet_ = static_cast<bullet::player_bullet*>( b );
          
            for( auto e : enemies_ )
            {
                auto enemy_ = static_cast<object::enemy_interface*>( e );
                if( enemy_->get_collision()->judge( player_bullet_->get_collision().get() ) )
                {
                    score_.add( enemy_->get_point() );
                    enemy_->collide( object_type::player_bullet );
                    player_bullet_->collide( object_type::enemy );
                }
            }
        }

        // player<->enemy
        if( no_judge_player > 0 ) // 無敵時間
        {
            no_judge_player -= delta_ * 100.; // 無敵時間はゲーム速度関係なし固定で引数使います
            if( no_judge_player < 0 )
            {
                no_judge_player = 0;
            }
        }
        else
        {
            for( auto i : enemies_ )
            {
                auto enemy_ = static_cast<object::enemy_interface*>( i );
                if( player_->get_collision()->judge( enemy_->get_collision().get() ) )
                {
                    enemy_->collide( object_type::player );
                    player_->collide( object_type::enemy );
                    //TODO: 自機減らす&僅かな判定無効時間
                    damage_player();
                }
            }

            // player<->enemy_bullets
            for( auto i : enemy_bullets_ )
            {
                auto enemy_bullet_ = static_cast<bullet::bullet_interface*>( i );
                if( player_->get_collision()->judge( enemy_bullet_->get_collision().get() ) )
                {
                    enemy_bullet_->collide( object_type::player );
                    player_->collide( object_type::enemy_bullet );
                    //TODO: 自機減らす&僅かな判定無効時間
                    damage_player();
                }
            }
        }

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

void judgement::damage_player()
{
    if( !remaining_.is_dead() )
    {
        get_child( get_child( get_parent( this ), "ui_layer" ), "remaining" )->getChildByTag( remaining_.get() )->removeFromParent();
        remaining_.decrease();
        no_judge_player = no_judge_player_time;
    }
}