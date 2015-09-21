/************************************************************************************//**
    @file	c2xa/c2xa_config.hpp
    @author    新ゝ月(NewNotMoon)
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
    static constexpr unsigned int score_digit = 8;
    static constexpr unsigned int max_score = 99999999;
    static constexpr unsigned int initial_player_remaining = 5;
    static constexpr unsigned int random_seed = 58378276;
    static constexpr float coin_probability = 0.001f;
    static constexpr unsigned int coin_point = 100;
}

#endif//C2XA_C2XA_CONFIG_HPP