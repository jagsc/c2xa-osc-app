/************************************************************************************//**
    @file   c2xa/scene/end_scene.hpp
    @author 新ゝ月かりな
    @date   created on 2015/09/29
****************************************************************************************/
#ifndef C2XA_SCENE_END_SCENE_HPP
#define C2XA_SCENE_END_SCENE_HPP

#include <cocos2d.h>

namespace c2xa
{
    namespace scene
    {
        class end_scene
            : public cocos2d::Scene
        {
        public:
            CREATE_FUNC( end_scene );
            virtual bool init() override;
        };
    }
}

#endif//C2XA_SCENE_END_SCENE_HPP
