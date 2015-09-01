/************************************************************************************//**
    @file	c2xa/scene/main_scene.hpp
    @brief	main scene

    C2XA OSCアプリのメインシーンです

    @author 新ゝ月かりな
    @date 2015/08/26
****************************************************************************************/
#ifndef C2XA_SCENE_MAIN_SCENE_HPP
#define C2XA_SCENE_MAIN_SCENE_HPP

#include <cocos2d.h>

#include <c2xa/scene/main/layer/object_layer.hpp>

//#include <scripting/lua-bindings/manual/CCLuaEngine.h>

namespace c2xa
{
    namespace scene
    {
        class main_scene
            : public cocos2d::Scene
        {
        private:
            bool end_ = false;

        public:
            CREATE_FUNC( main_scene );
            virtual bool init() override
            {
                using namespace cocos2d;

                if( !Scene::init() )
                {
                    return false;
                }
                scheduleUpdate();

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

                addChild( main::layer::object_layer::create() );

                auto keyboard_listener_ = EventListenerKeyboard::create();
                keyboard_listener_->onKeyPressed = [ & ]( EventKeyboard::KeyCode key_, Event* event_ )
                {
                    if( key_ == EventKeyboard::KeyCode::KEY_ESCAPE )
                    {
                        end_ = true;
                    }
                };
                auto dispatcher = Director::getInstance()->getEventDispatcher();
                dispatcher->addEventListenerWithSceneGraphPriority( keyboard_listener_, this );
                return true;
            }
            virtual void update( float ) override
            {
                if( end_ )
                {
                    Director::getInstance()->end();
                }
            }
        };
    }
}

#endif//C2XA_SCENE_MAIN_SCENE_HPP