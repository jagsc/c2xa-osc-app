/************************************************************************************//**
    @file   c2xa/scene/manual_scene.hpp
    @author 新ゝ月かりな
    @date   created on 2015/10/21
****************************************************************************************/
#ifndef C2XA_SCENE_MANUAL_SCENE_HPP
#define C2XA_SCENE_MANUAL_SCENE_HPP

#include <cocos2d.h>

namespace c2xa
{
    namespace scene
    {
        class manual_scene
            : public cocos2d::Scene
        {
        public:
            CREATE_FUNC( manual_scene );
            virtual bool init() override;
        };
    }
}

#endif//C2XA_SCENE_MANUAL_SCENE_HPP
