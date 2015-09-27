/************************************************************************************//**
    @file	c2xa/scene/main_scene/layer/ui_layer.hpp
    @author 新ゝ月(NewNotMoon)
    @date   created on 2015/09/19
****************************************************************************************/
#ifndef C2XA_SCENE_MAIN_SCENE_LAYER_UI_LAYER_HPP
#define C2XA_SCENE_MAIN_SCENE_LAYER_UI_LAYER_HPP

#include <array>

#include <cocos2d.h>

#include <c2xa/to_string_from_score.hpp>
#include <c2xa/object/player.hpp>
#include <c2xa/utility.hpp>

namespace c2xa
{
    namespace scene
    {
        namespace main
        {
            namespace layer
            {
                class score_display
                    : public cocos2d::Node
                {
                public:
                    CREATE_FUNC( score_display );
                    virtual bool init() override
                    {
                        using namespace cocos2d;
                        if( !Node::init() )
                        {
                            return false;
                        }
                        setName( "score_display" );

                        auto sf_ = Sprite::create( "img/score_font.png" );
                        for( int i = 0; i <= 9; ++i )
                        {
                            auto frame_ = SpriteFrame::createWithTexture(
                                sf_->getTexture(),
                                Rect{ i * score_font_width, 0, score_font_width, score_font_height } );
                            SpriteFrameCache::getInstance()
                                ->addSpriteFrame( frame_, std::move( get_cache_frame_name( i ) ) );
                        }

                        for( int i = 0; i < score_digit; ++i )
                        {
                            auto sprite_ = Sprite::createWithSpriteFrameName( "score_font_0" );
                            sprite_->setTag( i );
                            sprite_->setPosition( Vec2{
                                130 + i * ( score_font_width - score_font_margin_left ),
                                app_height - score_font_margin_top } );
                            sprite_->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
                            addChild( sprite_ );
                        }

                        return true;
                    }
                    void set_score( unsigned int score_number_ )
                    {
                        using namespace cocos2d;
                        auto score_ = to_string_from_score( score_number_ );
                        for( int i = 0; i < score_digit; ++i )
                        {
                            auto number_ = score_[ i ] - '0';
                            auto frame_ = SpriteFrameCache::getInstance()
                                ->getSpriteFrameByName( get_cache_frame_name( number_ ) );
                            auto sprite_ = static_cast<Sprite*>( getChildByTag( i ) );
                            sprite_->setTexture( frame_->getTexture() );
                            sprite_->setTextureRect( frame_->getRect() );
                            sprite_->setSpriteFrame( frame_ );
                        }
                    }

                private:
                    std::string get_cache_frame_name( int i_ )
                    {
                        return std::string{ "score_font_" } + one_digit_to_string( i_ );
                    }
                };
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

                        auto score_sprite_ = Sprite::create( "img/score_background.png" );
                        score_sprite_->setPosition( Vec2( 0, app_height ) );
                        score_sprite_->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );

                        addChild( score_sprite_ );
                        addChild( score_display::create() );

                        return true;
                    }
                    void set_score( unsigned int score_ )
                    {
                        get_child<score_display>( this, "score_display" )->set_score( score_ );
                    }
                };
            }
        }
    }
}
#endif//C2XA_SCENE_MAIN_SCENE_LAYER_UI_LAYER_HPP