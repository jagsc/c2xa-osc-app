
require "c2xa.action"
require "c2xa.moving"

-- 1クール
function c2xa_generate( object_layer )
    c2xa.enemies = object_layer:getChildByName( "enemies" )
    local sample_enemy1 = {
        move   = moving.straight( cc.p( 560, 960 ), cc.p( -50, 200 ) ),
        time   = 10
    }
    local sample_enemy2 = {
        move   = moving.straight( cc.p( -50, 600 ), cc.p( 610, 500 ) ),
        time   = 10
    }
    local sample_enemy3 = {
        move   = moving.curve( cc.p( 0, 0 ), cc.p( 300, 500 ), cc.p( 300, 300 ) ),
        time   = 10
    }
    local sample_enemy4 = {
        move   = moving.straight( cc.p( 610, 800 ), cc.p( -50, 700 ) ),
        time   = 10
    }
    local sample_enemy5 = {
        move   = moving.straight( cc.p( 100, 960 ), cc.p( 100, -50 ) ),
        time   = 10
    }
	local sample_enemy6 = {
        move   = moving.teleportation( cc.p( 0, 0 ), cc.p( 330, 420 ), cc.p( 300, 300 ) ),
        time   = 10
    }
    object_layer:runAction(
        action.sequence({
            action.create_enemy( sample_enemy1 ),
            action.wait( 2 ),
            action.create_enemy( sample_enemy4 ),
            action.wait( 0.5 ),
            action.create_enemy( sample_enemy4 ),
            action.wait( 0.5 ),
            action.create_enemy( sample_enemy4 ),
            action.wait( 0.5 ),
            action.wait( 2 ),
            action.create_enemy( sample_enemy2 ),
            action.wait( 0.5 ),
            action.create_enemy( sample_enemy2 ),
            action.wait( 0.5 ),
            action.create_enemy( sample_enemy2 ),
            action.wait( 2 ),
            action.create_enemy({ move = moving.straight( cc.p( 100, 960 ), cc.p( 100, -50 ) ), time = 10 } ),
            action.create_enemy({ move = moving.straight( cc.p( 200, 960 ), cc.p( 200, -50 ) ), time = 10 } ),
            action.wait( 2 ),
            action.create_enemy( sample_enemy2 ),
            action.wait( 2 ),
            action.create_enemy( sample_enemy2 ),
			action.wait( 2 ),
			action.create_enemy( sample_enemy3 ),
            action.wait( 1.5 ),
            action.create_enemy( sample_enemy6 ),
            action.wait( 0.5 ),
            action.create_enemy( sample_enemy3 ),
            cc.CallFunc:create(function()
                c2xa.set_magnification( c2xa.get_magnification() + 0.1 )
                c2xa_generate( object_layer )
            end)
        })
    )
end