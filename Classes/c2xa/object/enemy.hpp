/************************************************************************************//**
    @file    c2xa/object/enemies.hpp
    @brief    enemies

    @author    鈴木久貴
    @date      2015/09/12
****************************************************************************************/

#ifndef C2XA_OBJECT_ENEMY_HPP
#define C2XA_OBJECT_ENEMY_HPP

#include <cocos2d.h>

#include <AudioEngine.h>

#include <stdlib.h>
#include <c2xa/c2xa_config.hpp>
#include <c2xa/object/object.hpp>
#include <c2xa/bullet/enemy_bullet.hpp>
#include <c2xa/counter.hpp>
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
            : public enemy_interface
        {
        private:
            struct data
            {
                lua_State* state;
                int move_id;
                double duration = 0; // 初期化必須
                unsigned int point;

                struct fire
                {
                    enum class type
                    {
                        bullet,
                        time
                    } type_;
                    union info
                    {
                        struct bullet
                        {
                            int move_id;
                            double duration;
                        } bullet_;
                        double time_;
                        info( bullet b_ )
                        {
                            bullet_ = b_;
                        }
                        info( double t_ )
                        {
                            time_ = t_;
                        }
                    } info_;
                    fire( type type_, info info_ )
                        : type_{ type_ }
                        , info_{ info_ }
                    {
                    }
                };
                std::list<fire> fire_info_;
            };
            std::unique_ptr<data> data_;
            double progress_ = 0.;
            collision collision_;
            bool death_ = false;
            double previous_fire_time_ = 0;
            int previous_fire_number_ = 0;

        public:
            /// 引数のLuaステートに関数を登録します
            static void registrory_glue( lua_State* state_ )
            {
                static const struct luaL_Reg functions_[] ={
                    { "create_enemy", create_glue },
                    { "set_magnification", set_magnification_glue },
                    { "get_magnification", get_magnification_glue },
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

                //lua_getfield( state_, -1, "scheduler_id" );
                //CCASSERT( lua_isnumber( state_, -1 ), "" );
                //data_->scheduler_id = lua_tointeger( state_, -1 );

                //lua_getfield( state_, -1, "move" );
                //CCASSERT( lua_type( state_, -1 ) == LUA_TFUNCTION, "" );
                //data_->move_id = luaL_ref( state_, LUA_REGISTRYINDEX );

                data_->move_id = lua::to< lua::type::function >::from_table( state_, "move" );
                data_->duration = lua::to< lua::type::number >::from_table( state_, "duration" );
                data_->point = lua::to< lua::type::number >::from_table( state_, "point" );

                lua_getfield( state_, -1, "fire" );
                if( lua_type( state_, -1 ) == LUA_TTABLE )
                {
                    lua_pushnil( state_ ); //たぶんNilだと最初からって意味
                    while( lua_next( state_, -2 ) != 0 )
                    {
                        if( lua_type( state_, -1 ) == LUA_TTABLE )
                        {
                            // テーブルの場合は、弾だと判断します
                            auto move_id_ = lua::to< lua::type::function >::from_table( state_, "move" );
                            auto duration_ = lua::to< lua::type::number >::from_table( state_, "duration" );
                            data_->fire_info_.push_back( { data::fire::type::bullet, data::fire::info::bullet{ move_id_, duration_ } } );
                        }
                        else if( lua_type( state_, -1 ) == LUA_TNUMBER )
                        {
                            // 数値の場合は、待機時間だと判断します
                            auto time_ = lua_tonumber( state_, -1 );
                            data_->fire_info_.push_back( { data::fire::type::time, data::fire::info{ time_ } } );
                        }
                        else
                        {
                            CCASSERT( false, "値でもテーブルでもねえぞｵｲ！" );
                        }
                        lua_settop( state_, -2 );
                    }
                }
                lua_settop( state_, 0 );

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

                auto sprite_ = create_sprite_from_batch( get_current_scene(), "img/enemy_bugdroid.png" );
                sprite_->setPosition( get_position() );
                sprite_->setName( "sprite" );
                addChild( sprite_ );

                collision_ = create_collision_circul( sprite_ );

                fire();

                return true;
            }
            virtual void update( float delta_ ) override
            {
                progress_ += get_count();

                if( progress_ > data_->duration * 60 )
                {
                    clean();
                    return;
                }

                get_child<cocos2d::Sprite>( this, "sprite" )->setPosition( get_position() );
                fire();
            }

        private:
            void clean()
            {
                unscheduleUpdate();
                luaL_unref( data_->state, LUA_REGISTRYINDEX, data_->move_id );
                for( auto i : data_->fire_info_ )
                {
                    if( i.type_ == data::fire::type::bullet )
                    {
                        luaL_unref( data_->state, LUA_REGISTRYINDEX, i.info_.bullet_.move_id );
                    }
                }
                removeFromParent();
            }
            void fire()
            {
                if( !data_->fire_info_.empty() )
                {
                    auto info_ = data_->fire_info_.front();
                    while( info_.type_ != data::fire::type::bullet )
                    {
                        data_->fire_info_.pop_front();
                        previous_fire_time_ += info_.info_.time_ * 60;
                        if( data_->fire_info_.empty() )
                        {
                            break;
                        }
                        info_ = data_->fire_info_.front();
                    }
                    if( progress_ > previous_fire_time_ )
                    {
                        while( info_.type_ != data::fire::type::time )
                        {
                            std::unique_ptr<bullet::data> d_{ new bullet::data };
                            d_->state = data_->state;
                            d_->move_id = info_.info_.bullet_.move_id;
                            d_->duration = info_.info_.bullet_.duration;
                            auto pos_ = get_child<cocos2d::Sprite>( this, "sprite" )->getPosition();
                            d_->first_x = pos_.x;
                            d_->first_y = pos_.y;
                            auto bullet_ = bullet::enemy_bullet::create( std::move( d_ ) );
                            get_child( getParent()->getParent(), "enemy_bullets" )->addChild( bullet_ );
                            data_->fire_info_.pop_front();
                            if( data_->fire_info_.empty() )
                            {
                                break;
                            }
                            info_ = data_->fire_info_.front();
                        }
                        previous_fire_time_ = progress_;
                    }
                }
            }
            cocos2d::Vec2 get_position() const
            {
                // 呼び出す関数: move_idの参照先
                lua_rawgeti( data_->state, LUA_REGISTRYINDEX, data_->move_id );

                // 第一引数: 進捗率(0～100までのdouble)
                lua_pushnumber( data_->state, progress_ * 100 / ( data_->duration * 60 ) );

                // 呼び出し: 引数1: 戻り値1: 座標(xとyを持つテーブル)
                lua::call( data_->state, 1, 1 );

                double x = lua::to< lua::type::number >::from_table( data_->state, "x" );
                double y = lua::to< lua::type::number >::from_table( data_->state, "y" );

                lua_settop( data_->state, -2 );

                return std::move( cocos2d::Vec2 {
                    static_cast<float>( x ),
                    static_cast<float>( y )
                } );
            }
            unsigned int get_point() const override
            {
                return data_->point;
            }
            collision get_collision() const override
            {
                return collision_;
            }
            void collide( object_type type_ ) override
            {
                // 自機の弾か自機本体
                // ここもLuaから指定できるようにする？とりあえずまずはC++で実装
                switch( type_ )
                {
                case object_type::player:
                {
                    break;
                }
                case object_type::player_bullet:
                {
                    if( !death_ )
                    {
                        death_ = true;
                        auto sprite_1_ = create_sprite_from_batch( getParent()->getParent(), "img/main_scene_death_1.png" );
                        sprite_1_->setPosition( get_position() );
                        getParent()->getParent()->addChild( sprite_1_ );
                        sprite_1_->runAction(
                            cocos2d::Sequence::create(
                                cocos2d::CallFunc::create( [ sprite_1_ ]
                        {
                            cocos2d::experimental::AudioEngine::play2d( "sounds/death.mp3", false, 0.3f, nullptr );
                        } ),
                                cocos2d::DelayTime::create( 0.3f ),
                            cocos2d::FadeOut::create( 0.7f ),
                            cocos2d::CallFunc::create( [ sprite_1_ ]
                        {
                            sprite_1_->removeFromParent();
                        } ),
                            nullptr
                            ) );
                        auto sprite_2_ = create_sprite_from_batch( getParent()->getParent(), "img/main_scene_death_2.png" );
                        sprite_2_->setPosition( get_position() );
                        getParent()->getParent()->addChild( sprite_2_ );
                        sprite_2_->runAction(
                            cocos2d::Sequence::create(
                                cocos2d::Spawn::createWithTwoActions(
                                    cocos2d::RotateBy::create( 1.f, 2500.f ),
                                    cocos2d::Sequence::create( cocos2d::DelayTime::create( 0.3f ), cocos2d::FadeOut::create( 0.5f ), nullptr )
                                    ),
                                cocos2d::CallFunc::create( [ sprite_2_ ]
                        {
                            sprite_2_->removeFromParent();
                        } ),
                                nullptr
                            ) );
                        clean();
                        break;
                    }
                }
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