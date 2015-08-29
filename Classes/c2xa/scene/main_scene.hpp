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

namespace c2xa
{
    namespace scene
    {
        class main_scene
            : public cocos2d::Scene
        {
        public:
            static cocos2d::Scene* create()
            {
                using namespace cocos2d;

                auto a = Scene::create();
                //a->addChild( background_layer::create() );
                Size winSize = Director::getInstance()->getVisibleSize();
                auto _bg = LayerColor::create( Color4B::RED, winSize.width, winSize.height );
                a->addChild( _bg );
                a->addChild( main::layer::object_layer::create() );
                return a;
            }
            virtual bool init() override
            {
                if( !Scene::init() )
                {
                    return false;
                }
                return true;
            }
        };
    }
}

#endif//C2XA_SCENE_MAIN_SCENE_HPP