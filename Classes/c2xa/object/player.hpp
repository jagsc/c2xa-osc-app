/************************************************************************************//**
    @file   c2xa/object/player.hpp
    @author 新ゝ月(NewNotMoon)
    @date   created on 2015/08/29
****************************************************************************************/
#ifndef C2XA_OBJECT_PLAYER_HPP
#define C2XA_OBJECT_PLAYER_HPP

#include <cocos2d.h>
#include <c2xa/collision.hpp>
#include <c2xa/object/object.hpp>

namespace c2xa
{
    namespace object
    {
        /*!
            @class player

            プレイヤが操作するオブジェクト
        */
        class player
            : public cocos2d::Node
            , public object_interface
        {
        private:
            enum class move_state
            {
                NONE,
                LEFT,
                RIGHT
            } move_state_;

            float position_;
            float input_count_;
            bool  is_touch_;
            cocos2d::Point touch_position_;

            collision collision_;

        public:
            CREATE_FUNC( player );
            
        public:
            virtual bool init() override;
            virtual void update( float delta_ ) override;

            collision get_collision() const
            {
                return collision_;
            }
            unsigned int get_point() const override
            {
                // 自機にポイントはない
                return 0;
            }
            void collide( object_type ) override;

        private:
            void reset()
            {
                is_touch_ = false;
                input_count_ = 0.f;
                move_state_ = move_state::NONE;
            };
            void fire();
        };
    }
}

#endif//C2XA_OBJECT_PLAYER_HPP