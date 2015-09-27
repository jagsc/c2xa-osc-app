/************************************************************************************//**
    @file	 c2xa/scene/score_scene.hpp
    @author  新ゝ月(NewNotMoon)
    @date    created on 2015/09/20
****************************************************************************************/
#ifndef C2XA_SCENE_SCORE_SCENE_HPP
#define C2XA_SCENE_SCORE_SCENE_HPP

#include <cocos2d.h>

#include <c2xa/to_string_from_score.hpp>
#include <c2xa/utility.hpp>

namespace c2xa
{
    namespace scene
    {
        class score_scene
            : public cocos2d::Scene
        {
        private:
            unsigned int score_ = 0;

        public:
            static score_scene* create( unsigned int socre_ )
            {
                return create_template<score_scene>( socre_ );
            }
            virtual bool init( unsigned int s_ ) // !!! no-override !!!
            {
                using namespace cocos2d;

                if( !Scene::init() )
                {
                    return false;
                }

                score_ = s_;

                setName( "score_scene" );

                auto keyboard_listener_ = EventListenerKeyboard::create();
                keyboard_listener_->onKeyPressed = []( EventKeyboard::KeyCode key_, Event* event_ )
                {
                    cocos2d::Director::getInstance()->end();
                };
                auto dispatcher = Director::getInstance()->getEventDispatcher();
                dispatcher->addEventListenerWithSceneGraphPriority( keyboard_listener_, this );

                auto display_ = Label::createWithTTF( to_string_from_score( score_ ).data(), "fonts/arial.ttf", 48 );
                display_->setPosition( Point( app_width / 2 , app_height / 2 ) );
                display_->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
                addChild( display_ );

                return true;
            }
        };
    }
}

#endif//C2XA_SCENE_SCORE_SCENE_HPP