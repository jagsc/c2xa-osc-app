/************************************************************************************//**
    @file   c2xa/scene/main_scene.hpp
    @author 新ゝ月かりな
    @date   created on 2015/08/26
****************************************************************************************/
#ifndef C2XA_SCENE_MAIN_SCENE_HPP
#define C2XA_SCENE_MAIN_SCENE_HPP

#include <cocos2d.h>

//#include <scripting/lua-bindings/manual/CCLuaEngine.h>
//auto lua_engine_ = LuaEngine::getInstance();
//ScriptEngineManager::getInstance()->setScriptEngine( lua_engine_ );
//lua_engine_->executeScriptFile( "test.lua" );

//lua_State* l = lua_engine_->getLuaStack()->getLuaState();
//
//lua_getglobal( l, "helloLua" );
//tolua_pushusertype( l, this, "cc.Scene" );
//if( lua_pcall( l, 1, 0, 0 ) )
//    CCLOG( "error=%s", lua_tostring( l, lua_gettop( l ) ) );
//}

namespace c2xa
{
    namespace scene
    {
        class main_scene
            : public cocos2d::Scene
        {
        private:
            bool is_transitioning_ = false;

        public:
            CREATE_FUNC( main_scene );
            virtual bool init() override;
            void game_over();
        };
    }
}

#endif//C2XA_SCENE_MAIN_SCENE_HPP
