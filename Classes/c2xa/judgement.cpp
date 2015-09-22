
#include <c2xa/judgement.hpp>
#include <c2xa/scene/main_scene.hpp>
#include <c2xa/object/player.hpp>
#include <c2xa/utility.hpp>

using namespace c2xa;

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
        
        for( auto i : coins_ )
        {
            auto coin_ = static_cast<object::coin_interface*>( i );
            if( player_->get_collision()->judge( coin_->get_collision().get() ) )
            {
                score_.add( coin_->get_point() );
                cocos2d::experimental::AudioProfile p;
                p.maxInstances = 0; // 0は制限なしっぽい
                p.minDelay = 0; // 不明
                p.name = "get_coin"; // プロファイルに名前をつけられるっぽい？たぶんあとからプロファイルを取得したりする時に使う
                // play2d( ファイルパス, ループフラグ, 音量, プロファイルへのポインタ(nullptrでデフォルト) )
                cocos2d::experimental::AudioEngine::play2d( "sounds/get_coin.mp3", false, 0.3f, &p );
                i->removeFromParent();
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