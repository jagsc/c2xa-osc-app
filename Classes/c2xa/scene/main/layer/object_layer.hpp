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
                private:
                    cocos2d::Node* enemies_;
                    cocos2d::Node* enemy_bullets_;
                    cocos2d::Node* player_bullets_;

                public:
                    CREATE_FUNC( object_layer );
                    virtual bool init() override
                    {
                        if( !Layer::init() )
                        {
                            return false;
                        }
                        
                        setName( "object_layer" );

                        enemies_        = Node::create();
                        enemy_bullets_  = Node::create();
                        player_bullets_ = Node::create();
                        enemies_        ->setName( "enemies" );
                        enemy_bullets_  ->setName( "enemy_bullets" );
                        player_bullets_ ->setName( "player_bullets" );

                        addChild( enemies_ );
                        addChild( enemy_bullets_ );
                        addChild( player_bullets_ );
                        addChild( object::player::create() );
                        
                        return true;
                    }
                };
            }
        }
    }
}

#endif//C2XA_SCENE_MAIN_SCENE_LAYER_OBJECT_LAYER_HPP