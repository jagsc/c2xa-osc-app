#include <c2xa/judgement.hpp>
#include <c2xa/scene/main_scene.hpp>
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
        auto root_ = get_parent( this );
        auto player_position_ = cocos2d::utils::findChildren( *root_, "//player_sprite" ).front()->getPosition();
        // スコアカウント
        // 自機数カウント
        // ゲーム終了判断
        if( remaining_.is_dead() )
        {
            game_over_ = true;
            get_parent<scene::main_scene>( this )->game_over();
        }
    }
}