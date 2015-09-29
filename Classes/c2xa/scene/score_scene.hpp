/************************************************************************************//**
    @file	 c2xa/scene/score_scene.hpp
    @author  新ゝ月(NewNotMoon)
    @date    created on 2015/09/20
****************************************************************************************/
#ifndef C2XA_SCENE_SCORE_SCENE_HPP
#define C2XA_SCENE_SCORE_SCENE_HPP

#include <cocos2d.h>

namespace c2xa
{
    namespace scene
    {
        class score_scene
            : public cocos2d::Scene
        {
        private:
            unsigned int score_ = 0;

        public:
            static score_scene* create( unsigned int socre_ );
            virtual bool init( unsigned int ); // !!! no-override !!!
        private:
            unsigned int get_score() const
            {
                return score_;
            }
            void set_score( unsigned int score_number_ );
            std::string get_cache_frame_name( int i_ );
        };
    }
}

#endif//C2XA_SCENE_SCORE_SCENE_HPP