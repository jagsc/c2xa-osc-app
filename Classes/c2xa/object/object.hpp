/************************************************************************************//**
    @file	 c2xa/object/object.hpp
    @author  新ゝ月(NewNotMoon)
    @date    created on 2015/09/27
****************************************************************************************/
#ifndef C2XA_OBJECT_OBJECTN_HPP
#define C2XA_OBJECT_OBJECTN_HPP

#include <c2xa/collision.hpp>

namespace c2xa
{
    enum class object_type
    {
        player,
        player_bullet,
        enemy,
        enemy_bullet,
        coin,
    };

    class object_interface
    {
    public:
        /*!
            保持している接触判定情報を取得します
        */
        virtual auto get_collision() const -> collision = 0;

        /*!
            オブジェクトの持つスコアポイントを取得します
        */
        virtual auto get_point() const -> unsigned int = 0;

        /*!
            接触した時のコールバック関数です。引数は接触したオブジェクトの種類を示します。
        */
        virtual void collide( object_type ) = 0;
    };
}

#endif//C2XA_OBJECT_OBJECTN_HPP