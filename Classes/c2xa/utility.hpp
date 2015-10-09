/************************************************************************************//**
    @file    c2xa/utility.hpp
    @author  新ゝ月(NewNotMoon)
    @date    created on 2015/09/19
****************************************************************************************/
#ifndef C2XA_C2XA_UTILITY_HPP
#define C2XA_C2XA_UTILITY_HPP

#include <cocos2d.h>

#include <c2xa/counter.hpp>
#include <scripting/lua-bindings/manual/CCLuaEngine.h>

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
    counter* get_counter();
    double get_count();
    int get_magnification_glue( lua_State* state_ );
    void set_magnification( double mag_ );
    int set_magnification_glue( lua_State* state_ );

    namespace lua
    {
        static void print_stack( lua_State *L )
        {
            // スタック数を取得
            const int num = lua_gettop( L );
            if( num == 0 )
            {
                cocos2d::log( "No stack." );
                return;
            }
            for( int i = num; i >= 1; i-- )
            {
                cocos2d::log( "%03d(%04d): ", i, -num + i - 1 );
                int type = lua_type( L, i );
                switch( type )
                {
                case LUA_TNIL:
                    cocos2d::log( "NIL" );
                    break;
                case LUA_TBOOLEAN:
                    cocos2d::log( "BOOLEAN %s", lua_toboolean( L, i ) ? "true" : "false" );
                    break;
                case LUA_TLIGHTUSERDATA:
                    cocos2d::log( "LIGHTUSERDATA" );
                    break;
                case LUA_TNUMBER:
                    cocos2d::log( "NUMBER %f", lua_tonumber( L, i ) );
                    break;
                case LUA_TSTRING:
                    cocos2d::log( "STRING %s", lua_tostring( L, i ) );
                    break;
                case LUA_TTABLE:
                    cocos2d::log( "TABLE" );
                    break;
                case LUA_TFUNCTION:
                    cocos2d::log( "FUNCTION" );
                    break;
                case LUA_TUSERDATA:
                    cocos2d::log( "USERDATA" );
                    break;
                case LUA_TTHREAD:
                    cocos2d::log( "THREAD" );
                    break;
                }
            }
            cocos2d::log( "-----------------------------" );
        };

        void initialize_engine( cocos2d::LuaEngine* );
        void initialize_state( lua_State* );
        cocos2d::LuaEngine* get_engine();
        cocos2d::LuaStack* get_stack();
        lua_State* get_state();
        void call( lua_State* state_, int arg_num_, int return_num_ );
        void push_usertype( lua_State*, void* value_, char const* id_ );

        enum class type
            : int
        {
            none            = LUA_TNONE,
            nil             = LUA_TNIL,
            boolean         = LUA_TBOOLEAN,
            string          = LUA_TSTRING,
            table           = LUA_TTABLE,
            function        = LUA_TFUNCTION,
            thread          = LUA_TTHREAD,
            user_data       = LUA_TUSERDATA,
            light_user_data = LUA_TLIGHTUSERDATA,
            number          = LUA_TNUMBER,
            integer         = number << 3,
        };
        
        namespace detail
        {
            template< type type_ >
            struct dispatch;
            template<>
            struct dispatch< type::boolean >
            {
                using type = bool;
            };
            template<>
            struct dispatch< type::number >
            {
                using type = lua_Number;
            };
            template<>
            struct dispatch< type::integer >
            {
                using type = lua_Integer;
            };
            template<>
            struct dispatch< type::string >
            {
                using type = char const*;
            };
            template<>
            struct dispatch< type::function >
            {
                using type = int;
            };
        }

        template< type type_ >
        using return_type = typename detail::dispatch< type_ >::type;

        template< type >
        struct to;

        namespace detail
        {
            template< type type_ >
            struct extend_from_table
            {
                static return_type< type_ > from_table( lua_State* state_, char const* field_name_, int stack_index_ = -1 );
            };
        }

        template<>
        struct to< type::boolean >
            : detail::extend_from_table< type::boolean >
        {
            static return_type< type::boolean > from( lua_State* state_, int stack_index_ = -1 )
            {
                CCASSERT( lua_type( state_, stack_index_ ) == static_cast<int>( type::boolean ),
                    ( std::string{ "index is not boolean : " } +lua_typename( state_, stack_index_ ) ).c_str() );
                auto value_ = lua_toboolean( state_, stack_index_ ) != 0;
                lua_settop( state_, -2 );
                return value_;
            }
        };

        template<>
        struct to< type::number >
            : detail::extend_from_table< type::number >
        {
            static return_type< type::number > from( lua_State* state_, int stack_index_ = -1 )
            {
                CCASSERT( lua_type( state_, stack_index_ ) == static_cast<int>( type::number ),
                    ( std::string{ "index is not number : " } +lua_typename( state_, stack_index_ ) ).c_str() );
                auto value_ = lua_tonumber( state_, stack_index_ );
                lua_settop( state_, -2 );
                return value_;
            }
        };

        template<>
        struct to< type::integer >
            : detail::extend_from_table< type::integer >
        {
            static return_type< type::integer > from( lua_State* state_, int stack_index_ = -1 )
            {
                CCASSERT( lua_type( state_, stack_index_ ) == static_cast<int>( type::number ),
                    ( std::string{ "index is not integer : " } +lua_typename( state_, stack_index_ ) ).c_str() );
                auto value_ = lua_tointeger( state_, stack_index_ );
                lua_settop( state_, -2 );
                return value_;
            }
        };

        template<>
        struct to< type::string >
            : detail::extend_from_table< type::string >
        {
            static return_type< type::string > from( lua_State* state_, int stack_index_ = -1 )
            {
                CCASSERT( lua_type( state_, stack_index_ ) == static_cast<int>( type::string ),
                    ( std::string{ "index is not string : " } + lua_typename( state_, stack_index_ ) ).c_str() );
                auto value_ = lua_tolstring( state_, stack_index_, 0 );
                lua_settop( state_, -2 );
                return value_;
            }
        };

        template<>
        struct to< type::function >
            : detail::extend_from_table< type::function >
        {
            static return_type< type::function > from( lua_State* state_, int stack_index_ = -1 )
            {
                CCASSERT( lua_type( state_, stack_index_ ) == static_cast<int>( type::function ),
                    ( std::string{ "index is not function : " } + lua_typename( state_, stack_index_ ) ).c_str() );
                auto value_ = luaL_ref( state_, LUA_REGISTRYINDEX );
                // luaL_refはスタッククリーン不要な模様
                return value_;
            }
        };

        namespace detail
        {
            template< type type_ >
            return_type< type_ > extend_from_table< type_ >::from_table( lua_State* state_, char const* field_name_, int stack_index_ = -1 )
            {
                CCASSERT( lua_type( state_, stack_index_ ) == static_cast<int>( type::table ),
                    ( std::string{ "index is not table : " } +lua_typename( state_, stack_index_ ) ).c_str() );
                lua_getfield( state_, stack_index_, field_name_ );
                auto value_ = to< type_ >::from( state_, -1 );
                return value_;
            }
        }

    }
}

#endif//C2XA_C2XA_UTILITY_HPP