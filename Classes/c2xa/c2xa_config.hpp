/************************************************************************************//**
    @file	c2xa/c2xa_config.hpp
    @author    êVÅTåé(NewNotMoon)
    @date      2015/08/30
****************************************************************************************/
#ifndef C2XA_C2XA_CONFIG_HPP
#define C2XA_C2XA_CONFIG_HPP

#include <cocos2d.h>

namespace c2xa
{
    static constexpr float app_width  = 540.f;
    static constexpr float app_height = 960.f;
    static cocos2d::Rect const app_rectangle { 0, 0, app_width, app_height };
    static cocos2d::Size const app_size      { app_width, app_height };
}

#endif//C2XA_C2XA_CONFIG_HPP