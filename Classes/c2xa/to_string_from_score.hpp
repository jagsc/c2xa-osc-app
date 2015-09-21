/************************************************************************************//**
    @file	 c2xa/to_string_from_score.hpp
    @author  新ゝ月(NewNotMoon)
    @date    created on 2015/09/20
****************************************************************************************/
#ifndef C2XA_TO_STRING_FROM_SCORE_HPP
#define C2XA_TO_STRING_FROM_SCORE_HPP

#include <cstdio>
#include <string>

#include <c2xa/c2xa_config.hpp>

namespace c2xa
{
    static std::string to_string_from_score( unsigned int score_ )
    {
        assert( score_ <= max_score );
        char buffer_[ score_digit + 1 ];
        sprintf( buffer_, "%08d", score_ );
        //auto str_ = std::to_string( score_ );
        //str_ = std::string( score_digit - str_.length(), '0' ) + str_;
        return buffer_;
    }
}

#endif//C2XA_TO_STRING_FROM_SCORE_HPP