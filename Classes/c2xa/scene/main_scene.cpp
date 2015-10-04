/************************************************************************************//**
    @file   c2xa/scene/main_scene.cpp
    @author 新ゝ月かりな
    @date   created on 2015/09/29
****************************************************************************************/
#include <c2xa/judgement.hpp>

#include <c2xa/scene/main_scene.hpp>

#include <c2xa/scene/main/layer/ui_layer.hpp>
#include <c2xa/scene/main/layer/object_layer.hpp>
#include <c2xa/scene/main/layer/background_layer.hpp>

#include <c2xa/scene/score_scene.hpp>


using namespace cocos2d;
using namespace c2xa::scene;

bool main_scene::init()
{
    if( !Scene::init() )
    {
        return false;
    }

    setName( "main_scene" );
    addChild( main::layer::ui_layer::create(), 3 );
    addChild( main::layer::object_layer::create(), 2 );
    addChild( main::layer::background_layer::create(), 1 );

    addChild( judgement::create() );

    lua::execute_file( "scripts/test.lua" );
    lua::get_global( "helloLua" );
    lua::push_usertype( this, "cc.Scene" );
    lua::call( 1, 0 );

    return true;
}
void main_scene::game_over()
{
    if( !is_transitioning_ )
    {
        // メモ:
        //  update内でreplaseしてバグに悩んだ。
        //  トランジションの場合、replaceは即時に切り替わるわけではないので
        //  updateが呼ばれ続ける。そして大量にreplaceされて大変な事になる。
        //  ここみたいにコールバックのような状況か、トランジション中のフラグを用意
        //  するなど気をつける必要がある。後者が適切か。
        Director::getInstance()
            ->replaceScene(
                TransitionFade::create(
                    1.0f,
                    scene::score_scene::create(
                        get_child<judgement>( this, "judgement" )->get_score() ) ) );
        is_transitioning_ = true;
    }
}