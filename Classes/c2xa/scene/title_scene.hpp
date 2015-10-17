#ifndef C2XA_SCENE_TITLE_SCENE_HPP
#define C2XA_SCENE_TITLE_SCENE_HPP


#include <cocos2d.h>

namespace c2xa
{
    namespace scene
    {
        class title_scene
            : public cocos2d::Scene
        {
        public:
            CREATE_FUNC( title_scene );
            virtual bool init() override;
        };
    }
    namespace layer{
        class title_btn
            : public cocos2d::Layer
            {
        public:
            CREATE_FUNC(title_btn);
            virtual bool init() override;
//                void mycallback(Ref* pSender);

            };
        class title_background
            : public cocos2d::Layer
            {
            public:
            CREATE_FUNC(title_background);
            virtual bool init() override;
            };
    }

}

#endif //C2XA_SCENE_TITLE_SCENE_HPP
