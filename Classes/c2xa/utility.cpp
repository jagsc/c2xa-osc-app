
#include <c2xa/utility.hpp>

#include <c2xa/object/enemy.hpp>

namespace c2xa
{
    namespace lua
    {
        void initialize_engine( cocos2d::LuaEngine* engine_ )
        {
            auto manager_ = cocos2d::ScriptEngineManager::getInstance();
            if( manager_->getScriptEngine() == nullptr )
            {
                manager_->setScriptEngine( engine_ );
            }
            engine_->addSearchPath( "scripts" );
            engine_->executeScriptFile( "scripts/cocos/init.lua" );
        }
        cocos2d::LuaEngine* get_engine()
        {
            return cocos2d::LuaEngine::getInstance();
        }
        cocos2d::LuaStack* get_stack()
        {
            return get_engine()->getLuaStack();
        }
        void initialize_state( lua_State* state_ )
        {
            object::enemy::registrory_glue( state_ );
        }
        lua_State* get_state()
        {
            return get_stack()->getLuaState();
        }
        void call( lua_State* state_, int arg_num_, int return_num_ )
        {
            if( lua_pcall( state_, arg_num_, return_num_, 0 ) )
            {
                cocos2d::log( "error=%s", lua_tostring( state_, lua_gettop( state_ ) ) );
            }
        }
        void push_usertype( lua_State* state_, void* value_, char const* id_ )
        {
            tolua_pushusertype( state_, value_, id_ );
        }
    }
}