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

#include <c2xa/scene/main/layer/ui_layer.hpp>
#include <c2xa/scene/main/layer/object_layer.hpp>
#include <c2xa/scene/main/layer/background_layer.hpp>

#include <c2xa/scene/score_scene.hpp>

#include <c2xa/judgement.hpp>

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
                setName( "main_scene" );
                addChild( main::layer::ui_layer::create(),         3 );
                addChild( main::layer::object_layer::create(),     2 );
                addChild( main::layer::background_layer::create(), 1 );

                addChild( judgement::create() );

                return true;
            }
            virtual void update( float ) override
            {
            }
            void game_over()
            {
                using namespace cocos2d;
                // メモ:
                //  update内でreplaseしてバグに悩んだ。
                //  トランジションの場合、replaceは即時に切り替わるわけではないので
                //  updateが呼ばれ続ける。そして大量にreplaceされて大変な事になる。
                //  ここみたいにコールバックのような状況か、トランジション中のフラグを用意
                //  するなど気をつける必要がある。後者が適切か。
                Director::getInstance()
                    ->replaceScene(
                        TransitionFade::create(
                            3.0f,
                            scene::score_scene::create(
                                get_child<judgement>( this, "judgement" )->get_score() ) ) );
            }
        };
    }
}

#endif//C2XA_SCENE_MAIN_SCENE_HPP
