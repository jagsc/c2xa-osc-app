/************************************************************************************//**
    @file	 c2xa/collision.hpp
    @author  新ゝ月(NewNotMoon)
    @date    created on 2015/09/20
****************************************************************************************/
#ifndef C2XA_COLLISION_HPP
#define C2XA_COLLISION_HPP

#include <memory>

#include <cocos2d.h>

namespace c2xa
{
    namespace detail
    {
        class collision_circul;
        class collision
        {
        public:
            /*!
                @param[in] s_ 接触判定オブジェクト
                @return 接触したかを示す真偽値
            */
            virtual bool judge( collision const* ) const = 0;
            /*!
                @param[in] s_ 円形接触判定オブジェクト
                @return 接触したかを示す真偽値
            */
            virtual bool dispatch( collision_circul const* ) const = 0;
        };
        class collision_circul
            : public collision
        {
        private:
            cocos2d::Node* node_;
            float square_radius_;

        public:
            /*!
                @param[in] r_ 円形接触判定の半径
                @param[in] n_ 円形接触判定の対象オブジェクト
             */
            collision_circul( float r_, cocos2d::Node* n_ )
                : node_{ n_ }
                , square_radius_{ r_ * r_ }
            {
                node_->retain();
            }
            /*!
                @param[in] n_ 円形接触判定の対象オブジェクト
            */
            collision_circul( cocos2d::Node* n_ )
                : node_{ n_ }
            {
                node_->retain();
                auto s_ = ( node_->getContentSize() / 2 );
                float f_ = s_.height > s_.width ? s_.height : s_.width;
                square_radius_ = f_ * f_;
            }
            ~collision_circul()
            {
                node_->release();
            }
            /*!
                @return コンストラクタで渡された対象オブジェクトの座標
            */
            cocos2d::Vec2 const& get_position() const
            {
                return node_->getPosition();
            }
            /*!
                @return 円形接触判定の半径の二乗
            */
            float get_square_radius() const
            {
                return square_radius_;
            }
            /*!
                @param[in] s_ 接触判定オブジェクト
                @return 接触したかを示す真偽値
            */
            virtual bool judge( collision const* s_ ) const override
            {
                return s_->dispatch( this );
            }
            /*!
                @param[in] s_ 円形接触判定オブジェクト
                @return 接触したかを示す真偽値
            */
            virtual bool dispatch( collision_circul const* s_ ) const override
            {
                auto t_ = node_->getPosition() - s_->get_position();
                auto f_ = t_.x * t_.x + t_.y * t_.y;
                return s_->get_square_radius() + square_radius_ >= f_ ? true : false;
            }
        };
    }
    using collision = std::shared_ptr<detail::collision>;

    /*!
        円形接触判定オブジェクトを作成します。直接コンストラクタを呼ばず、この関数で作ってください。
        この関数の引数は、そのままコンストラクタに渡されます。引数はコンストラクタを参照してください。
    */
    template< typename... ARGS >
    collision create_collision_circul( ARGS&&... args_ )
    {
        return std::make_shared<detail::collision_circul>( std::forward<ARGS>( args_ )... );
    }
}

#endif//C2XA_COLLISION_HPP