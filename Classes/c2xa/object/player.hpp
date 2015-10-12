/************************************************************************************//**
    @file   c2xa/object/player.hpp
    @author 新ゝ月(NewNotMoon)
    @date   created on 2015/08/29
****************************************************************************************/
#ifndef C2XA_OBJECT_PLAYER_HPP
#define C2XA_OBJECT_PLAYER_HPP

#include <cocos2d.h>
#include <c2xa/collision.hpp>
#include <c2xa/object/object.hpp>
#include <c2xa/utility.hpp>

namespace c2xa
{
    namespace object
    {
        /*!
            @class player

            プレイヤが操作するオブジェクト
        */
        class player
            : public cocos2d::Node
            , public object_interface
        {
        private:
            enum class move_state
            {
                NONE,
                LEFT,
                RIGHT
            } move_state_;

            float position_;
            float input_count_;
            bool  is_touch_;
            cocos2d::Point touch_position_;

            collision collision_;

        public:
            CREATE_FUNC( player );
            
        public:
            virtual bool init() override;
            virtual void update( float delta_ ) override;

            float get_position() const
            {
                return position_;
            }
            collision get_collision() const
            {
                return collision_;
            }
            unsigned int get_point() const override
            {
                // 自機にポイントはない
                return 0;
            }
            void collide( object_type ) override;

        private:
            void reset();
            void fire();

        public:
            // Luaから呼ぶための関数です
            static int lua_get_player_position( lua_State* state_ )
            {
                auto p = get_current_scene();
                if( p != nullptr )
                {
                    p = p->getChildByName( "object_layer" );
                    if( p != nullptr )
                    {
                        p = p->getChildByName( "player" );
                        if( p != nullptr )
                        {
                            lua_pushnumber( state_, static_cast<double>( static_cast<player*>( p )->get_position() ) );
                            return 1;
                        }
                    }
                }
                lua_pushnumber( state_, 0. );
                return 1;
            }
            static void registrory_glue( lua_State* state_ )
            {
                static const struct luaL_Reg functions_[] ={
                    { "get_player_position", lua_get_player_position },
                    { nullptr, nullptr }
                };
                luaL_register( state_, "c2xa", functions_ );
            }
        };
    }
}

#endif//C2XA_OBJECT_PLAYER_HPP