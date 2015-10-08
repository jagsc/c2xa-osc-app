/************************************************************************************//**
    @file	c2xa/scene/main/counter.hpp
    @author 新ゝ月(NewNotMoon)
    @date   created on 2015/09/19
****************************************************************************************/
#ifndef C2XA_SCENE_MAIN_COUNTER_HPP
#define C2XA_SCENE_MAIN_COUNTER_HPP

#include <cocos2d.h>

namespace c2xa
{
    class counter
    {
    private:
        double magnification_ = 1.;

    public:
        void set_magnification( double mag_ )
        {
            magnification_ = mag_;
        }
        double get()
        {
            return static_cast<double>( cocos2d::Director::getInstance()->getDeltaTime() ) * 100. * magnification_;
        }
        float operator()()
        {
            return get();
        }
    };
}
#endif//C2XA_SCENE_MAIN_COUNTER_HPP