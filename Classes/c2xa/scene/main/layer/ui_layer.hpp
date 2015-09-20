/************************************************************************************//**
    @file	c2xa/scene/main_scene/layer/ui_layer.hpp
    @author    新ゝ月(NewNotMoon)
    @date      2015/09/19
****************************************************************************************/
#ifndef C2XA_SCENE_MAIN_SCENE_LAYER_UI_LAYER_HPP
#define C2XA_SCENE_MAIN_SCENE_LAYER_UI_LAYER_HPP

#include <cocos2d.h>

#include <c2xa/to_string_from_score.hpp>
#include <c2xa/object/player.hpp>

namespace c2xa
{
    namespace scene
    {
        namespace main
        {
            namespace layer
            {
                class ui_layer
                    : public cocos2d::Layer
                {
                public:
                    CREATE_FUNC( ui_layer );
                    virtual bool init() override
                    {
                        using namespace cocos2d;
                        if( !Layer::init() )
                        {
                            return false;
                        }

                        setName( "ui_layer" );

                        // 暫定でarialフォント使用 いずれ画像に差し替え
                        auto score_text_ = Label::createWithTTF( "SCORE", "fonts/arial.ttf", 48 );
                        score_text_->setPosition( Point( 20, app_height - 20 ) );
                        score_text_->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
                        addChild( score_text_ );

                        auto score_number_ = Label::createWithTTF( "000000", "fonts/arial.ttf", 48 );
                        score_number_->setName( "score" );
                        score_number_->setPosition( Point( 220, app_height - 20 ) );
                        score_number_->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
                        addChild( score_number_ );

                        set_score( 293 ); //! 暫定

                        return true;
                    }
                    void set_score( unsigned int score_ )
                    {
                        static_cast<cocos2d::Label*>( getChildByName( "score" ) )
                            ->setString( to_string_from_score( score_ ) );
                    }
                };
            }
        }
    }
}
#endif//C2XA_SCENE_MAIN_SCENE_LAYER_UI_LAYER_HPP