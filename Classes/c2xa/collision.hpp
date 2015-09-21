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
            virtual bool judge( collision const* ) const = 0;
            virtual bool dispatch( collision_circul const* ) const = 0;
        };
        class collision_circul
            : public collision
        {
        private:
            cocos2d::Node* node_;
            float square_radius_;

        public:
            collision_circul( float r_, cocos2d::Node* n_ )
                : node_{ n_ }
                , square_radius_{ r_ * r_ }
            {
                node_->retain();
            }
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
            cocos2d::Vec2 const& get_position() const
            {
                return node_->getPosition();
            }
            float get_square_radius() const
            {
                return square_radius_;
            }
            virtual bool judge( collision const* s_ ) const override
            {
                return s_->dispatch( this );
            }
            virtual bool dispatch( collision_circul const* s_ ) const override
            {
                auto t_ = node_->getPosition() - s_->get_position();
                auto f_ = t_.x * t_.x + t_.y * t_.y;
                return s_->get_square_radius() + square_radius_ >= f_ ? true : false;
            }
        };
    }
    using collision = std::shared_ptr<detail::collision>;

    template< typename... ARGS >
    collision create_collision_circul( ARGS&&... args_ )
    {
        return std::make_shared<detail::collision_circul>( std::forward<ARGS>( args_ )... );
    }
}

#endif//C2XA_COLLISION_HPP