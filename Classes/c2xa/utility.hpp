/************************************************************************************//**
    @file	c2xa/utility.hpp
    @author    新ゝ月(NewNotMoon)
    @date      2015/09/19
****************************************************************************************/
#ifndef C2XA_C2XA_UTILITY_HPP
#define C2XA_C2XA_UTILITY_HPP

#include <cocos2d.h>

namespace c2xa
{
#ifdef _DEBUG
    static void log( char const* mes_ )
    {
        cocos2d::log( mes_ );
    }
#else//_DEBUG
    static void log( char const* )
    {
    }
#endif//_DEBUG
    template< typename TYPE, typename... ARGS >
    inline TYPE* create_template( ARGS&&... a )
    {
        auto p = new TYPE{};

        if( p && p->init( std::forward<ARGS>( a )... ) )
        {
            p->autorelease();
            return p;
        }
        else
        {
            delete p;
            return nullptr;
        }
    };
    static cocos2d::Node* get_current_scene()
    {
        return cocos2d::Director::getInstance()->getRunningScene()->getChildren().at( 1 );
    }
    static cocos2d::SpriteBatchNode* add_sprite_batch( cocos2d::Node* node_, std::string const& path_, std::string const& id_ )
    {
        auto images_ = node_->getChildByName( "images" );
        if( images_ == nullptr )
        {
            images_ = cocos2d::Node::create();
            images_->setName( "images" );
            node_->addChild( images_ );
        }
        auto batch_ = static_cast<cocos2d::SpriteBatchNode*>( images_->getChildByName( id_ ) );
        if( batch_ == nullptr )
        {
            batch_ = cocos2d::SpriteBatchNode::create( path_ );
            batch_->setName( id_ );
            images_->addChild( batch_, 1 );
        }
#ifdef _DEBUG
        else
        {
            log( "conflict sprite batch identity." );
        }
#endif//_DEBUG
        return batch_;
    }
    static cocos2d::Sprite* create_sprite_from_batch( cocos2d::Node* node_, std::string const& id_ )
    {
        auto batch_ =
            static_cast<cocos2d::SpriteBatchNode*>( node_
                ->getChildByName( "images" )
                ->getChildByName( id_ ) );
        return cocos2d::Sprite::createWithTexture( batch_->getTexture() );
    }
}

#endif//C2XA_C2XA_UTILITY_HPP