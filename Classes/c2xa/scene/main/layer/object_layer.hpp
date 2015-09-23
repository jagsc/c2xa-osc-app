/************************************************************************************//**
    @file	c2xa/scene/main_scene/layer/object_layer.hpp
    @brief	object_layer

    @author    êVÅTåé(NewNotMoon)
    @date      2015/08/29
****************************************************************************************/
#ifndef C2XA_SCENE_MAIN_SCENE_LAYER_OBJECT_LAYER_HPP
#define C2XA_SCENE_MAIN_SCENE_LAYER_OBJECT_LAYER_HPP

#include <cocos2d.h>

#include <c2xa/object/player.hpp>
#include <c2xa/object/coin.hpp>
#include <c2xa/object/super_coin.hpp>
#include <c2xa/object/enemy.hpp>

namespace c2xa
{
    namespace scene
    {
        namespace main
        {
            namespace layer
            {
                class object_layer
                    : public cocos2d::Layer
                {
                public:
                    CREATE_FUNC( object_layer );
                    virtual bool init() override
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
                        enemies_        ->setName( "enemies" );
                        enemy_bullets_  ->setName( "enemy_bullets" );
                        player_bullets_ ->setName( "player_bullets" );
                        coins_          ->setName( "coins" );

                        addChild( enemies_ );
                        addChild( enemy_bullets_ );
                        addChild( player_bullets_ );
                        addChild( coins_ );
                        addChild( object::player::create() );
                        
                        return true;
                    }
                    virtual void update( float ) override
                    {
                        static std::mt19937 engine_( random_seed );
                        std::bernoulli_distribution dist_( coin_probability );
                        if( dist_( engine_ ) )
                        {
                            get_child( this, "coins" )->addChild( object::coin::create() );
                            //get_child( this, "enemies_" )->addChild( object::enemy1::create() );
                        }
                        std::bernoulli_distribution dist_super_coin_( super_coin_probability );
                        if( dist_super_coin_( engine_ ) )
                        {
                            get_child( this, "coins" )->addChild( object::super_coin::create() );
                            //get_child( this, "enemies_" )->addChild( object::enemy2::create() );
                        }
                    }
                };
            }
        }
    }
}

#endif//C2XA_SCENE_MAIN_SCENE_LAYER_OBJECT_LAYER_HPP