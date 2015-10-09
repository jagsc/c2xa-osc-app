
require "c2xa.action"
require "c2xa.moving"

-- 1クール
function c2xa_generate( object_layer )
    c2xa.enemies = object_layer:getChildByName( "enemies" )
    local sample_enemy1 = {
        move   = moving.straight( cc.p( 560, 960 ), cc.p( 0, 0 ) ),
        time   = 8
    }
    local sample_enemy2 = {
        move   = moving.straight( cc.p( 0, 0 ), cc.p( 560, 960 ) ),
        time   = 4
    }
    local sample_enemy3 = {
        move   = moving.teleportation( cc.p( 0, 0 ), cc.p( 560, 960 ), cc.p( 300, 300 ) ),
        time   = 4
    }
    local sample_enemy4 = {
        move   = moving.straight( cc.p( 560, 0 ), cc.p( 0, 960 ) ),
        time   = 4
    }
    object_layer:runAction(
        action.sequence({
            action.create_enemy( sample_enemy1 ),
            action.wait( 2 ),
            action.create_enemy( sample_enemy3 ),
            action.wait( 2 ),
            action.create_enemy( sample_enemy4 ),
            action.wait( 2 ),
            action.create_enemy( sample_enemy2 ),
            action.wait( 2 ),
            action.create_enemy( sample_enemy1 ),
            action.create_enemy( sample_enemy2 ),
            action.wait( 2 ),
            action.create_enemy( sample_enemy2 ),
            action.wait( 2 ),
            action.create_enemy( sample_enemy2 )
        })
    )
end