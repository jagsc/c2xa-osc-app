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
                int update_function;
                double duration = 0; // 初期化必須
                unsigned int point;
                double start_x;
                double start_y;
                double goal_x;
                double goal_y;

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
                            int update_function;
                            double duration;
                            double goal_x;
                            double goal_y;
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
            static void registrory_glue( lua_State* state_ );
            /// この関数はLuaに登録されます
            /// @attention Luaから呼び出す関数です
            ///
            /// この関数はLuaスタックトップに1つの値が積まれていることを期待します
            /// その引数はテーブル型であり、適切なフィールドを持っていると期待します
            static int create_glue( lua_State* state_ );
            virtual bool init( std::unique_ptr<data>&& d_ ); // no-override;
            virtual void update( float delta_ ) override;

        private:
            void clean();
            void fire();
            cocos2d::Vec2 get_position() const;
            unsigned int get_point() const override;
            collision get_collision() const override;
            void collide( object_type type_ ) override;
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