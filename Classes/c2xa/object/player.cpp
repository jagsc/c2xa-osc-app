#include <c2xa/object/player.hpp>
#include <c2xa/bullet/test_player_bullet.hpp>

using namespace c2xa::object;

void player::fire()
{
    getParent()->getChildByName("player_bullets")->addChild( bullet::test_player_bullet::create( position_ ) );
}