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

#include <c2xa/bullet/lua.hpp>

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
                //a->addChild( background_layer::create() );
                //Size winSize = Director::getInstance()->getVisibleSize();
                //auto _bg = LayerColor::create( Color4B::RED, winSize.width, winSize.height );
                //addChild( _bg );
                addChild( main::layer::object_layer::create() );

                auto keyboard_listener_ = EventListenerKeyboard::create();
                keyboard_listener_->onKeyPressed = [ & ]( EventKeyboard::KeyCode key_, Event* event_ )
                {
                    if( key_ == EventKeyboard::KeyCode::KEY_ESCAPE )
                    {
                        Director::getInstance()->end();
                    }
                };
                auto dispatcher = Director::getInstance()->getEventDispatcher();
                dispatcher->addEventListenerWithSceneGraphPriority( keyboard_listener_, this );
                return true;
            }
        };
    }
}

#endif//C2XA_SCENE_MAIN_SCENE_HPP