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
    class object_interface
    {
    public:
        /*!
            保持している接触判定情報を取得します
        */
        virtual auto get_collision() const -> collision    = 0;

        /*!
            オブジェクトのポイントを取得します
        */
        virtual auto get_point()     const -> unsigned int = 0;

        /*!
            接触した時のコールバック関数です
        */
        virtual void on_collide() = 0;
    };
}

#endif//C2XA_OBJECT_OBJECTN_HPP