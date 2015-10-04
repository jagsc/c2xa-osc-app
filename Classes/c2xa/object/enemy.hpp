/************************************************************************************//**
    @file    c2xa/object/enemies.hpp
    @brief    enemies

    @author    鈴木久貴
    @date      2015/09/12
****************************************************************************************/

#ifndef C2XA_OBJECT_ENEMY_HPP
#define C2XA_OBJECT_ENEMY_HPP

#include <cocos2d.h>

#include <stdlib.h>
#include <c2xa/c2xa_config.hpp>
#include <c2xa/object/object.hpp>
#include <c2xa/utility.hpp>

namespace c2xa
{
    namespace object
    {    
        class enemy_interface
            : public cocos2d::Node
            , public object_interface
        {
        };

        /// @author 新ゝ月かりな
        /// @date created on 2015/10/04
        /// このクラスは基本的にLuaからのみ作成できます
        class enemy
            : public cocos2d::Node
        {
        private:
            struct data
            {
                lua_State* state;
                int move_id;
                int scheduler_id;
            };
            std::unique_ptr<data> data_;
            double progress_ = 0.;

        public:
            /// 引数のLuaステートに関数を登録します
            static void registrory_glue( lua_State* state_ )
            {
                static const struct luaL_Reg functions_[] ={
                    { "create_enemy", create_glue },
                    { nullptr, nullptr }
                };
                luaL_register( state_, "c2xa", functions_ );
            }
            /// この関数はLuaに登録されます
            /// @attention Luaから呼び出す関数です
            ///
            /// この関数はLuaスタックトップに1つの値が積まれていることを期待します
            /// その引数はテーブル型であり、適切なフィールドを持っていると期待します
            static int create_glue( lua_State* state_ )
            {
                std::unique_ptr<data> data_{ new data }; // make_uniqueはC++11にないので我慢…

                data_->state = state_;

                lua_getfield( state_, -1, "scheduler_id" );
                CCASSERT( lua_isnumber( state_, -1 ), "" );
                data_->scheduler_id = lua_tointeger( state_, -1 );

                lua_getfield( state_, -2, "move" );
                CCASSERT( lua_type( state_, -1 ) == LUA_TFUNCTION, "" );
                data_->move_id = luaL_ref( state_, LUA_REGISTRYINDEX );

                auto enemy_ = create_template<enemy>( std::move( data_ ) );

                lua::push_usertype( state_, enemy_, "cc.Node" );

                return 1; // Luaの受け取る戻り値の数は1(スタックトップの1つ目が戻り値として認識される)
            }
            virtual bool init( std::unique_ptr<data>&& d_ ) // no-override
            {
                if( !Node::init() )
                {
                    return false;
                }
                data_ = std::move( d_ );
                scheduleUpdate();

                auto sprite_ = create_sprite_from_batch( get_current_scene(), "img/player_bugdroid.png" );
                sprite_->setPosition( cocos2d::Point::ZERO );
                sprite_->setName( "sprite" );
                addChild( sprite_ );

                return true;
            }
            virtual void update( float delta_ ) override
            {
                progress_ += delta_ * 100.f; //TODO: delta_ * 100 の部分は暫定
                // Luaの内部表現はdoubleなのでdoubleに合わせます

                if( progress_ > 100.f )
                {
                    cleanup();
                    removeFromParent();
                    return;
                }

                // 呼び出す関数: move_idの参照先
                lua_rawgeti( data_->state, LUA_REGISTRYINDEX, data_->move_id );

                // 第一引数: 始点(xとyを持つテーブル)
                lua_createtable( data_->state, 0, 2 );
                lua_pushnumber( data_->state, 0 );
                lua_setfield( data_->state, -2, "x" );
                lua_pushnumber( data_->state, 0 );
                lua_setfield( data_->state, -2, "y" );

                // 第一引数: 終点(xとyを持つテーブル)
                lua_createtable( data_->state, 0, 2 );
                lua_pushnumber( data_->state, 560 );
                lua_setfield( data_->state, -2, "x" );
                lua_pushnumber( data_->state, 960 );
                lua_setfield( data_->state, -2, "y" );

                // 第三引数: 進捗率(0～100までのdouble)
                lua_pushnumber( data_->state, progress_ );

                // 呼び出し: 戻り値: 座標(xとyを持つテーブル)
                lua::call( data_->state, 3, 1 );

                lua_getfield( data_->state, -1, "x" );
                CCASSERT( lua_isnumber( data_->state, -1 ), "" );
                double x = lua_tonumber( data_->state, -1 );

                lua_getfield( data_->state, -2, "y" );
                CCASSERT( lua_isnumber( data_->state, -1 ), "" );
                double y = lua_tonumber( data_->state, -1 );

                get_child<cocos2d::Sprite>( this, "sprite" )->setPosition(
                    cocos2d::Vec2{
                        static_cast<float>( x ),
                        static_cast<float>( y )
                    } );
            }
            ~enemy()
            {
                cleanup();
            }

        private:
            void cleanup()
            {
                if( data_ != nullptr )
                {
                    luaL_unref( data_->state, LUA_REGISTRYINDEX, data_->move_id );
                    cocos2d::Director::getInstance()->getScheduler()->unscheduleScriptEntry( data_->scheduler_id );
                    data_.reset( nullptr );
                }
            }
        };

        //class base_enemy : public enemy_interface
        //{
        //protected :
        //    int score;
        //    cocos2d::Vec2 position_;
        //    cocos2d::Vec2 move_direction_;
        //    float move_speed_;
        //    cocos2d::Sprite *enemy_sprite_;
        //    float time_ = 0.f;
        //    collision collision_;
        //    //破壊
        //    void delete_enemy_node();
        //};

        //class enemy1 : public base_enemy {

        //public:
        //    ~enemy1()
        //    {
        //        enemy_sprite_->release();
        //    }

        //    CREATE_FUNC(enemy1);

        //public:
        //    //初期化関数
        //    virtual bool init() override
        //    {
        //        using namespace cocos2d;

        //        if (!Node::init())
        //        {
        //            return false;
        //        }

        //        scheduleUpdate();
        //        setName( "enemy1" );
        //        int random = 1 + (int)( rand() *( 10 - 1 + 1.f ) / (1.f + RAND_MAX) );
        //        position_ ={ app_width / random,app_height };
        //        enemy_sprite_ = Sprite::create( "img/player_bugdroid.png" );
        //        enemy_sprite_->setPosition(position_);
        //        enemy_sprite_->setName( "enemy_sprite" );
        //        addChild(enemy_sprite_);

        //        collision_ = create_collision_circul( enemy_sprite_ );
        //        move_speed_ = 1.f;
        //        return true;
        //    }
        //    //アップデート関数
        //    virtual void update( float delta_ ) override
        //    {
        //        if( position_.y < -50.f )
        //        {
        //            delete_enemy_node();
        //        }
        //        else
        //        {
        //            move(delta_);
        //            time_ += ( delta_ * 100 );
        //            if( time_ > 100 )
        //            {
        //                fire(delta_);
        //                time_ = 0.f;
        //            }
        //        }
        //    }

        //    unsigned int get_point() const override
        //    {
        //        return 1;
        //    }
        //    collision get_collision() const override
        //    {
        //        return collision_;
        //    }
        //    void collide( object_type ) override
        //    {
        //        delete_enemy_node();
        //    }

        //    void fire(float time) override;
        //    void move(float time) override;
        //};


        //class enemy2 : public base_enemy
        //{
        //private:
        //    int dir;
        //public:
        //    ~enemy2()
        //    {
        //        enemy_sprite_->release();
        //    }

        //    CREATE_FUNC( enemy2 );

        //public:
        //    //初期化関数
        //    virtual bool init() override
        //    {
        //        using namespace cocos2d;

        //        if( !Node::init() )
        //        {
        //            return false;
        //        }

        //        scheduleUpdate();
        //        setName( "enemy2" );
        //        dir = rand() % 2;
        //        int random = 1 + (int)( rand() *( 3 - 1 + 1.f ) / ( 1.f + RAND_MAX ) );
        //        if( dir == 0 )
        //        {
        //            position_ ={ app_width ,app_height/random };
        //        }
        //        else
        //        {
        //            position_ ={ 0,app_height/random };
        //        }
        //        enemy_sprite_ = Sprite::create( "img/player_bugdroid.png" );
        //        enemy_sprite_->setPosition( position_ );
        //        enemy_sprite_->setName( "enemy_sprite" );
        //        addChild( enemy_sprite_ );

        //        collision_ = create_collision_circul( enemy_sprite_ );
        //        move_speed_ = 1.f;
        //        return true;
        //    }
        //    //アップデート関数
        //    virtual void update( float delta_ ) override
        //    {

        //        if( position_.y < -50.f )
        //        {
        //            delete_enemy_node();
        //        }
        //        else
        //        {
        //            move(delta_);
        //            time_ += ( delta_ * 100 );
        //            if( time_ > 100 )
        //            {
        //                fire(delta_);
        //                time_ = 0.f;
        //            }
        //        }
        //    }

        //    unsigned int get_point() const override
        //    {
        //        return 1;
        //    }
        //    collision get_collision() const override
        //    {
        //        return collision_;
        //    }
        //    void collide( object_type ) override
        //    {
        //        delete_enemy_node();
        //    }

        //    void fire( float time ) override;
        //    void move( float time ) override;
        //};

        //class enemy3 : public base_enemy
        //{
        //private:
        //    typedef enum
        //    {
        //        APPEAR, ACT, DISAPPEAR
        //    }STATE_TYPE;
        //    STATE_TYPE state;
        //public:
        //    ~enemy3()
        //    {
        //        enemy_sprite_->release();
        //    }

        //    CREATE_FUNC( enemy3 );

        //public:
        //    //初期化関数
        //    virtual bool init() override
        //    {
        //        using namespace cocos2d;

        //        if( !Node::init() )
        //        {
        //            return false;
        //        }
        //        state = STATE_TYPE::APPEAR;
        //        scheduleUpdate();
        //        setName( "enemy3" );
        //        
        //        int random = 1 + (int)( rand() *( 20 - 1 + 1.f ) / ( 1.f + RAND_MAX ) );
        //        position_ ={ app_width/random ,app_height / random };
        //        enemy_sprite_ = Sprite::create( "img/player_bugdroid.png" );
        //        enemy_sprite_->setPosition( position_ );
        //        enemy_sprite_->setName( "enemy_sprite" );
        //        addChild( enemy_sprite_ );

        //        collision_ = create_collision_circul( enemy_sprite_ );
        //        move_speed_ = 5.f;
        //        return true;
        //    }
        //    //アップデート関数
        //    virtual void update( float delta_ ) override
        //    {
        //        move(delta_);
        //        time_ += ( delta_ * 100 );
        //        if( time_ > 100 )
        //        {
        //            fire(delta_);
        //            time_ = 0.f;
        //        }
        //    }

        //    unsigned int get_point() const override
        //    {
        //        return 1;
        //    }
        //    collision get_collision() const override
        //    {
        //        return collision_;
        //    }
        //    void collide( object_type ) override
        //    {
        //        delete_enemy_node();
        //    }

        //    void fire( float time ) override;
        //    void move( float time ) override;
        //};

        //class enemy4 : public base_enemy
        //{
        //private:
        //    int index;      //軌道データを指す
        //    int num_orbit;  //軌道データの要素数
        //    float* vx; float* vy;   //軌道データ
        //    float* timer;   //各軌道データの時間

        //public:
        //    ~enemy4()
        //    {
        //        enemy_sprite_->release();
        //    }

        //    CREATE_FUNC( enemy4 );

        //public:
        //    //初期化関数
        //    virtual bool init() override
        //    {
        //        using namespace cocos2d;

        //        if( !Node::init() )
        //        {
        //            return false;
        //        }
        //        scheduleUpdate();
        //        setName( "enemy4" );

        //        index=0;
        //        num_orbit = 15 + (int)( rand() *( 20 - 15 + 1.f ) / ( 1.f + RAND_MAX ) );
        //        vx = new float[ num_orbit ];
        //        vy = new float[ num_orbit ];
        //        timer = new float[ num_orbit ];
        //        set_orbit();

        //        int random = 1 + (int)( rand() *( 10 - 1 + 1.f ) / ( 1.f + RAND_MAX ) );
        //        position_ ={ app_width / random ,app_height};
        //        enemy_sprite_ = Sprite::create( "img/player_bugdroid.png" );
        //        enemy_sprite_->setPosition( position_ );
        //        enemy_sprite_->setName( "enemy_sprite" );
        //        addChild( enemy_sprite_ );

        //        collision_ = create_collision_circul( enemy_sprite_ );
        //        move_speed_ = 2.f;
        //        return true;
        //    }
        //    //アップデート関数
        //    virtual void update( float delta_ ) override
        //    {
        //        move( delta_ );
        //        time_ += ( delta_ * 100 );
        //        if( time_ > 100 )
        //        {
        //            fire( delta_ );
        //            time_ = 0.f;
        //        }
        //    }

        //    unsigned int get_point() const override
        //    {
        //        return 1;
        //    }
        //    collision get_collision() const override
        //    {
        //        return collision_;
        //    }
        //    void collide( object_type ) override
        //    {
        //        delete_enemy_node();
        //    }

        //    void fire( float time ) override;
        //    void move( float time ) override;
        //    void set_orbit()
        //    {
        //        for( int i=0; i < num_orbit; i++ )
        //        {
        //            if( position_.x > app_width / 2 )
        //            {
        //                vx[ i ] = -2.f + ( rand() *( 0.f - ( -2.f ) + 1.f ) / ( 1.f + RAND_MAX ) );
        //            }
        //            else
        //            {
        //                vx[ i ] = 0.f + ( rand() *( 2.f - ( 0.f ) + 1.f ) / ( 1.f + RAND_MAX ) );
        //            } 
        //            if( i < 5 )
        //            {
        //                vy[ i ] = -2.f;
        //            }
        //            else
        //            {
        //                vy[ i ] = -2.f + ( rand() *( 0.f - ( -2.f ) + 1.f ) / ( 1.f + RAND_MAX ) );
        //            }
        //            timer[i] = 1.f + ( rand() *( 1.3f -  1.f  + 1.f ) / ( 1.f + RAND_MAX ) );
        //        }
        //    }
        //};
    }
}

#endif//C2XA_OBJECT_ENEMY_HPP