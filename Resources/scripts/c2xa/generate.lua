
require "c2xa.action"
require "c2xa.moving"
require "c2xa.bullet"

-- 1クール
function c2xa_generate( object_layer )
    c2xa.enemies = object_layer:getChildByName( "enemies" )

    -------------- 

    local sample_bullet1 = {
        update   = moving.homing,
        goal     = cc.p( 300, -50 ),
        duration = 5,
    }
    
    -------------- 
    
    local enemy_pattern1 = {
        update = moving.straight,
        start  = cc.p( 610, 800 ),
        goal   = cc.p( -50, 700 ),
        duration = 10,
        fire = { 3, sample_bullet1, 3, sample_bullet1 }
    }
    local enemy_pattern2 = {
        update = moving.homing,
        start  = cc.p( -50, 600 ),
        goal   = cc.p( 610, 500 ),
        duration = 10,
        fire = { 3, sample_bullet1, 3, sample_bullet1 },
    }
    -------------- 

    object_layer:runAction(
        action.sequence({
            action.create_enemy( enemy_pattern1 ),
            action.wait( 0.5 ),
            action.create_enemy( enemy_pattern1 ),
            action.wait( 0.5 ),
            action.create_enemy( enemy_pattern1 ),
            action.wait( 4 ),
            action.create_enemy( enemy_pattern2 ),
            action.wait( 0.5 ),
            action.create_enemy( enemy_pattern2 ),
            action.wait( 0.5 ),
            action.create_enemy( enemy_pattern2 ),
            cc.CallFunc:create(function()
                c2xa.set_magnification( c2xa.get_magnification() + 0.1 )
                c2xa_generate( object_layer )
            end)
        })
    )
end