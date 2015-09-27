/************************************************************************************//**
    @file	 c2xa/to_string_from_score.hpp
    @author  新ゝ月(NewNotMoon)
    @date    created on 2015/09/20
****************************************************************************************/
#ifndef C2XA_TO_STRING_FROM_SCORE_HPP
#define C2XA_TO_STRING_FROM_SCORE_HPP

#include <cstdio>
#include <array>

#include <c2xa/c2xa_config.hpp>

namespace c2xa
{
    /*!
        符号なし整数を文字の配列に変換します。ただし、変換可能な数値はスコアの最大値以下です。
    */
    static std::array< char, score_digit + 1 > to_string_from_score( unsigned int score_ )
    {
        CCASSERT( score_ <= max_score, "Max score is 8-digit." );
        std::array<char, score_digit + 1> buffer_;
        sprintf( buffer_.data(), "%08d", score_ );
        //auto str_ = std::to_string( score_ );
        //str_ = std::string( score_digit - str_.length(), '0' ) + str_;
        return buffer_;
    }
    /*!
        符号なし整数を文字に変換します。ただし、変換可能な数値は一桁の数値のみです。
    */
    static char one_digit_to_string( unsigned int i_ )
    {
        CCASSERT( i_ <= 9 && i_ >= 0, "Param is need one digit." );
        char buffer_[ 2 ];
        sprintf( buffer_, "%1d", i_ );
        //auto str_ = std::to_string( score_ );
        //str_ = std::string( score_digit - str_.length(), '0' ) + str_;
        return buffer_[ 0 ];
    }
}

#endif//C2XA_TO_STRING_FROM_SCORE_HPP