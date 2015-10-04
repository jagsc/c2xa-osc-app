/************************************************************************************//**
    @file    c2xa/utility.hpp
    @author  新ゝ月(NewNotMoon)
    @date    created on 2015/09/19
****************************************************************************************/
#ifndef C2XA_C2XA_UTILITY_HPP
#define C2XA_C2XA_UTILITY_HPP

#include <cocos2d.h>

namespace c2xa
{
    /*!
        @fn log

        簡易ログ出力関数です。フォーマット機能はありません。
        デバッグビルドでは削除されます。

        @param[in] mes_ 表示する文字列
    */
#ifdef COCOS2D_DEBUG
    static void log( char const* mes_ )
    {
        cocos2d::log( mes_, 0 );
    }
#else//COCOS2D_DEBUG
    static void log( char const* )
    {
    }
#endif//COCOS2D_DEBUG
    /*!
        Cocos2d-xのCREATE_FUNCの代わりとなるヘルパ関数です。
        init関数が複数の引数を保つ場合でも使えます。
    */
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
    /*!
        現在のシーンを取得する関数です。
        @attention シーン遷移中にこの関数が呼ばれると、遷移後のシーンを返します。
                   必ずしも想定したシーンが返されるとは限りません。なるべく相対的に取得してください。

        @return 現在のシーン

    */
    static cocos2d::Node* get_current_scene()
    {
        return cocos2d::Director::getInstance()->getRunningScene();
    }
    /*!
        引数で渡されたノードの子要素をIDで取得します。また、テンプレート引数を指定した場合、
        その型にキャストして返します。
    */
    template< typename TYPE = cocos2d::Node >
    static TYPE* get_child( cocos2d::Node const* parent_, std::string const& id )
    {
#ifdef COCOS2D_DEBUG
        auto n_ = parent_->getChildByName( id );
        CCASSERT( n_ != nullptr, ( "child not found: " + id ).c_str() );
        return static_cast<TYPE*>( n_ );
#else//COCOS2D_DEBUG
        return static_cast<TYPE*>( parent_->getChildByName( id ) );
#endif//COCOS2D_DEBUG
    }
    /*!
        引数で渡されたノードの親要素を取得します。また、テンプレート引数を指定した場合、
        その型にキャストして返します。
    */
    template< typename TYPE = cocos2d::Node >
    static TYPE* get_parent( cocos2d::Node* child_ )
    {
        return static_cast<TYPE*>( child_->getParent() );
    }
    /*!
        現在のシーンのルートからバッチノードを探索し、スプライトバッチノードからスプライトを生成します。

        @return スプライト
    */
    static cocos2d::Sprite* create_sprite_from_batch( cocos2d::Node* node_, std::string const& path_ )
    {
        auto images_ = node_->getChildByName( "images" );
        if( images_ == nullptr )
        {
            images_ = cocos2d::Node::create();
            images_->setName( "images" );
            node_->addChild( images_ );
        }
        auto batch_ = static_cast<cocos2d::SpriteBatchNode*>( images_->getChildByName( path_ ) );
        if( batch_ == nullptr )
        {
            batch_ = cocos2d::SpriteBatchNode::create( path_ );
            batch_->setName( path_ );
            images_->addChild( batch_, 1 );
        }
        return cocos2d::Sprite::createWithTexture( batch_->getTexture() );
    }
}

#endif//C2XA_C2XA_UTILITY_HPP