
require "c2xa.action"
require "c2xa.moving"

-- 1クール
function c2xa_generate( node )
    local sample_enemy1 = {
        enemies = node,
        move    = moving.straight( cc.p( 0, 0 ), cc.p( 560, 960 ) ),
        time    = 8
    }
    local sample_enemy2 = {
        enemies = node,
        move    = moving.straight( cc.p( 0, 0 ), cc.p( 560, 960 ) ),
        time    = 4
    }
    node:runAction(
        action.sequence({
            action.create_enemy( sample_enemy1 ),
            action.wait( 2 ),
            action.create_enemy( sample_enemy2 ),
            action.wait( 2 ),
            action.create_enemy( sample_enemy1 ),
            action.wait( 2 ),
            action.create_enemy( sample_enemy2 ),
            action.wait( 2 ),
            action.create_enemy( sample_enemy1 )
        })
    )
end