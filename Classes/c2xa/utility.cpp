
#include <c2xa/utility.hpp>


namespace c2xa
{
    namespace lua
    {
        cocos2d::LuaEngine* get_engine()
        {
            static bool flag_ = false;
            auto engine_ = cocos2d::LuaEngine::getInstance();
            auto manager_ = cocos2d::ScriptEngineManager::getInstance();
            if( manager_->getScriptEngine() == nullptr )
            {
                manager_->setScriptEngine( engine_ );
            }
            if( !flag_ )
            {
                flag_ = true;
                engine_->addSearchPath( "scripts" );
                engine_->executeScriptFile( "scripts/cocos/init.lua" );
            }
            return engine_;
        }
        cocos2d::LuaStack* get_stack()
        {
            return get_engine()->getLuaStack();
        }
        lua_State* get_state()
        {
            return get_stack()->getLuaState();
        }
        void execute_file( char const* path_ )
        {
            auto engine_ = get_engine();
            engine_->executeScriptFile( path_ );
        }
        void call( int arg_num_, int return_num_ )
        {
            auto state_ = get_state();
            if( lua_pcall( state_, arg_num_, return_num_, 0 ) )
            {
                cocos2d::log( "error=%s", lua_tostring( state_, lua_gettop( state_ ) ) );
            }
        }
        void get_global( char const* id_ )
        {
            lua_getglobal( get_state(), id_ );
        }
        void push_usertype( void* value_, char const* id_ )
        {
            tolua_pushusertype( get_state(), value_, id_ );
        }
    }
}