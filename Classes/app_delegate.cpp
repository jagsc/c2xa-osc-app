#include <app_delegate.hpp>
#include <c2xa/main_scene.hpp>

using namespace cocos2d;

app_delegete::app_delegete() {

}

app_delegete::~app_delegete()
{
}

void app_delegete::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool app_delegete::applicationDidFinishLaunching() {
    auto director = Director::getInstance(); // require initialize
    auto glview = director->getOpenGLView();
    if(!glview) {
        //glview = GLViewImpl::create("My Game");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        glview = GLViewImpl::createWithRect( "My Game", Rect( 0, 0, 540, 960 ), 1 );
#else
        glview = GLViewImpl::create( "My Game" );
#endif
        director->setOpenGLView(glview);
    }

    // 当面はFPS表示します
    director->setDisplayStats(true);

    // 60FPSでまずは進めます
    director->setAnimationInterval(1.0 / 60);


    static const cocos2d::Size designResolutionSize = cocos2d::Size( 540, 960 );

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
    //director->setContentScaleFactor( 0.5 );

    register_all_packages();

    // create a scene. it's an autorelease object
    auto scene = c2xa::main_scene::create();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void app_delegete::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void app_delegete::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
