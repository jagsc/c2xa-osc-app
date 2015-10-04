/************************************************************************************//**
    @file   c2xa/scene/main/layer/object_layer.cpp
    @author 新ゝ月かりな
    @date   created on 2015/10/03
****************************************************************************************/

#include <c2xa/scene/main/layer/object_layer.hpp>

#include <c2xa/object/player.hpp>
#include <c2xa/object/coin.hpp>
#include <c2xa/object/super_coin.hpp>

#include <scripting/lua-bindings/manual/CCLuaEngine.h>

using namespace c2xa::scene::main::layer;

bool object_layer::init()
{
    if( !Layer::init() )
    {
        return false;
    }

    scheduleUpdate();
    setName( "object_layer" );

    auto enemies_        = Node::create();
    auto enemy_bullets_  = Node::create();
    auto player_bullets_ = Node::create();
    auto coins_          = Node::create();
    enemies_->setName( "enemies" );
    enemy_bullets_->setName( "enemy_bullets" );
    player_bullets_->setName( "player_bullets" );
    coins_->setName( "coins" );

    addChild( enemies_ );
    addChild( enemy_bullets_ );
    addChild( player_bullets_ );
    addChild( coins_ );
    addChild( object::player::create() );

    lua::get_engine()->executeScriptFile( "scripts/c2xa/generate.lua" );
    lua_getglobal( lua::get_state(), "c2xa_generate" );
    lua::push_usertype( lua::get_state(), enemies_, "cc.Node" );
    lua::call( lua::get_state(), 1, 0 );

    return true;
}

void object_layer::update( float )
{
    static std::mt19937 engine_( random_seed );
    std::bernoulli_distribution dist_( coin_probability );
    if( dist_( engine_ ) )
    {
        get_child( this, "coins" )->addChild( object::coin::create() );
    }
    std::bernoulli_distribution dist_super_coin_( super_coin_probability );
    if( dist_super_coin_( engine_ ) )
    {
        get_child( this, "coins" )->addChild( object::super_coin::create() );
    }
}
