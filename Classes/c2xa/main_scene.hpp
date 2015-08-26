/************************************************************************************//**
    @file	c2xa/main_scene.hpp
    @details 
****************************************************************************************/
#ifndef C2XA_MAIN_SCENE_HPP
#define C2XA_MAIN_SCENE_HPP

#include "cocos2d.h"

namespace c2xa
{
    using namespace cocos2d;

    template< typename TYPE, typename... ARGS >
    TYPE* create_template( ARGS&&... a )
    {
        auto p = new TYPE{};

        if( p && p->init( std::forward<ARGS>( a )... ) )
        {
            p->autorelease();
            return p;
        }
        else
        {
            delete  p;
            p = nullptr;
            return nullptr;
        }
    };

    class main_scene
        : public cocos2d::Scene
    {
    private:
        //class background_layer
        //    : public cocos2d::Layer
        //{
        //private:
        //    cocos2d::Sprite* sprite1_;
        //    cocos2d::Sprite* sprite2_;
        //public:
        //    CREATE_FUNC( background_layer );
        //    virtual bool init() override
        //    {
        //        if( !Layer::init() )
        //        {
        //            return false;
        //        }
        //        this->scheduleUpdate();

        //        sprite1_ = cocos2d::Sprite::create( "HelloWorld.png" );
        //        sprite2_ = cocos2d::Sprite::create( "HelloWorld.png" );

        //        this->sprite1_->setPosition( cocos2d::Vec2( 100, 0 ) );
        //        this->sprite2_->setPosition( cocos2d::Vec2( 100, 320 ) );

        //        this->addChild( sprite1_, 1 );
        //        this->addChild( sprite2_, 2 );

        //        return true;
        //    }
        //    virtual void update( float delta ) override
        //    {
        //        static int c = 0;
        //        ++c %= ( 60 * 4 );
        //        this->sprite1_->setPositionY( ( 480 * c / ( 60 * 4 ) ) );
        //        this->sprite2_->setPositionY( ( 480 * c / ( 60 * 4 )) + 320 );
        //    }
        //};

        
        class test_player_bullet
            : public cocos2d::Node
        {
        private:
            float   duration_ = 320.f; // frame
            float   count_ = 0.f;
            Vec2    target_position_;
            Vec2    first_position_;
            Sprite* bullet_;

            /*
                Actionは割り込みに弱い(できなくはないが、コールバック地獄になるので危険)
                弾は移動中に接触時に消えるという割り込みの可能性があるため、素直に座標持って自分で動かす
                または、割り込み可能なActionを作るか(作業量増、拡張性増、見通し減)
            */

        public:
            static test_player_bullet* create( float player_x_ )
            {
                return create_template<test_player_bullet>( player_x_ );
            }
            virtual bool init( float player_x_ ) // !!! no-override !!!
            {
                if( !Node::init() )
                {
                    return false;
                }
                this->scheduleUpdate();

                target_position_ = { player_x_, 800 }; // とりあえず画面外
                first_position_  = { player_x_, 100 };

                bullet_ = Sprite::create( "CloseSelected.png" );
                bullet_->setPosition( first_position_ );
                addChild( bullet_ );

                return true;
            }
            virtual void update( float delta_ ) override
            {
                count_ += 1.f * ( delta_ * 100 );

                if( count_ > duration_ )
                {
                    removeFromParent();
                }
                else
                {
                    bullet_->setPosition( ( target_position_ - first_position_ ) * count_ / duration_ + first_position_ );
                }
            }
        };

        class main_layer
            : public cocos2d::Layer
        {
        public:
            CREATE_FUNC( main_layer );
            virtual bool init() override
            {
                if( !Layer::init() )
                {
                    return false;
                }
                this->addChild( player::create() );
                return true;
            }
        private:
            class player
                : public cocos2d::Node
            {
            public:
                static const int y_position = 100;

            private:
                Sprite* i_;
                float   position_;
                enum class move_state
                {
                    NONE,
                    LEFT,
                    RIGHT
                } move_state_;
                bool touch_;
                bool tap_;
                bool direction_;
                int touch_count_;
                Point touch_position_;

            public:
                CREATE_FUNC( player );
                virtual bool init() override
                {
                    if( !Node::init() )
                    {
                        return false;
                    }
                    this->scheduleUpdate();


                    i_ = Sprite::create( "CloseNormal.png" );
                    position_ = y_position;
                    i_->setPosition( Vec2( position_, y_position ) );
                    this->addChild( i_ );

                    auto dispatcher = Director::getInstance()->getEventDispatcher();
                    auto listener = EventListenerKeyboard::create();

                    listener->onKeyPressed = [ & ]( EventKeyboard::KeyCode key_, Event* event_ )
                    {
                        switch( key_ )
                        {
                        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                            move_state_ = move_state::LEFT; break;
                        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                            move_state_ = move_state::RIGHT; break;
                        case EventKeyboard::KeyCode::KEY_UP_ARROW:
                            getParent()->addChild( test_player_bullet::create( position_ ) ); break;
                        case EventKeyboard::KeyCode::KEY_ESCAPE:
                            this->removeFromParent(); break; // 自分を消す(アクションも削除されるので注意)
                        }
                    };

                    listener->onKeyReleased = [ & ]( EventKeyboard::KeyCode key_, Event* event_ )
                    {
                        if( ( key_ == EventKeyboard::KeyCode::KEY_LEFT_ARROW && move_state_ == move_state::LEFT )
                         || ( key_ == EventKeyboard::KeyCode::KEY_RIGHT_ARROW && move_state_ == move_state::RIGHT ) )
                        {
                            move_state_ = move_state::NONE;
                        }
                    };

                    dispatcher->addEventListenerWithSceneGraphPriority( listener, this );

                    auto touch_listener_ = EventListenerTouchOneByOne::create();

                    touch_listener_->onTouchBegan = [ & ]( Touch* t_, Event* )
                    {
                        touch_count_ = 1;
                        touch_ = true;
                        tap_ = true;
                        touch_position_ = t_->getLocation();
                        if( position_ < touch_position_.x )
                        {
                            direction_ = true;
                        }
                        else
                        {
                            direction_ = false;
                        }
                        return true;
                    };

                    touch_listener_->onTouchMoved = [ & ]( Touch* t_, Event* )
                    {
                        touch_position_ = t_->getLocation();
                        if( position_ < touch_position_.x )
                        {
                            direction_ = true;
                        }
                        else
                        {
                            direction_ = false;
                        }
                    };

                    touch_listener_->onTouchCancelled = [ & ]( Touch* t_, Event* )
                    {
                        touch_count_ = 0;
                        touch_ = false;
                        tap_ = false;
                    };

                    touch_listener_->onTouchEnded = [ & ]( Touch* t_, Event* )
                    {
                        if( tap_ )
                        {
                            getParent()->addChild( test_player_bullet::create( position_ ) );
                        }
                        touch_count_ = 0;
                        touch_ = false;
                        tap_ = false;
                    };

                    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority( touch_listener_, this );


                    return true;
                }
                virtual void update( float delta_ ) override
                {
                    switch( move_state_ )
                    {
                    case move_state::LEFT:
                        position_ -= 1.f * ( delta_ * 100 ); i_->setPositionX( position_ ); break;
                    case move_state::RIGHT:
                        position_ += 1.f * ( delta_ * 100 ); i_->setPositionX( position_ ); break;
                    }
                    if( touch_ )
                    {
                        ++touch_count_;
                        if( touch_count_ > 10 )
                        {
                            tap_ = false;
                        }
                        if( !tap_ )
                        {
                            if( direction_ )
                            {
                                position_ += 1.f * ( delta_ * 100 ); i_->setPositionX( position_ );
                                if( touch_position_.x <= position_ )
                                {
                                    i_->setPositionX( touch_position_.x );
                                    touch_count_ = 0;
                                    touch_ = false;
                                    tap_ = false;
                                }
                            }
                            else
                            {
                                position_ -= 1.f * ( delta_ * 100 ); i_->setPositionX( position_ );
                                if( touch_position_.x >= position_ )
                                {
                                    i_->setPositionX( touch_position_.x );
                                    touch_count_ = 0;
                                    touch_ = false;
                                    tap_ = false;
                                }
                            }
                        }
                    }
                }
            };
        };
    public:
        static cocos2d::Scene* create()
        {
            auto a = cocos2d::Scene::create();
            //a->addChild( background_layer::create() );
            Size winSize = Director::getInstance()->getVisibleSize();
            auto _bg = LayerColor::create( Color4B::RED, winSize.width, winSize.height );
            a->addChild( _bg );
            a->addChild( main_layer::create() );
            return a;
        }
        virtual bool init() override
        {
            if( !Scene::init() )
            {
                return false;
            }
            //this->scheduleUpdate();
            return true;
        }
        //virtual void update( float delta ) override
        //{

        //}
    };
}

#endif//C2XA_MAIN_SCENE_HPP