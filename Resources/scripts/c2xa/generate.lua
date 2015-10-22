
require "c2xa.action"
require "c2xa.moving"

-- 1クール
function c2xa_generate( object_layer )
    c2xa.enemies = object_layer:getChildByName( "enemies" )

    -------------- 
    
    local bullet_normal = {
        update   = moving.straight,
        goal     = cc.p( 50, -50 ),
        duration = 5,
    }
    local bullet_player = {
        update   = moving.player,
        goal     = cc.p( 0, -50 ),
        duration = 4,
    }
    local bullet_player_high = {
        update   = moving.player,
        goal     = cc.p( 0, -50 ),
        duration = 2,
    }
    local bullet_homing = {
        update   = moving.homing,
        goal     = cc.p( 0, -50 ), -- ホーミングは特殊弾なのでこいつは無視されます
        duration = 8,
    }
    local bullet_homing_high = {  -- はやいバージョン
        update   = moving.homing,
        goal     = cc.p( 0, -50 ),
        duration = 4,
    }
    
    -------------- 
    
    local enemy_pattern000 = {
        update = moving.straight,
        start  = cc.p( 200, 960 ),
        goal   = cc.p( 200, -50 ),
        duration = 10,
        fire = { 3, bullet_player }
    }
    local enemy_pattern100 = {
        update = moving.homing,
        start  = cc.p( 400, 960 ),
        goal   = cc.p( 400, -50 ),
        duration = 10,
        fire = { 1, bullet_normal, 1, bullet_normal, 1, bullet_normal }
    }
    local enemy_pattern200 = {
        update = moving.homing,
        start  = cc.p( 400, 960 ),
        goal   = cc.p( 400, -50 ),
        duration = 10,
        fire = { 1, bullet_player, 1, bullet_player, 1, bullet_player }
    }
    local enemy_pattern210 = {
        update = moving.homing,
        start  = cc.p( 200, 960 ),
        goal   = cc.p( 200, -50 ),
        duration = 10,
        fire = { 1, bullet_player, 1, bullet_player, 1, bullet_player }
    }
    local enemy_pattern300 = {
        update = moving.wait_once,
        start  = cc.p( -50, 600 ),
        goal   = cc.p( 610, 600 ),
        duration = 10,
        fire = { 3, bullet_player_high, 0.5, bullet_player_high, 0.5, bullet_player_high, 0.5, bullet_player_high }
    }
    local enemy_pattern400 = {
        update = moving.wait_once,
        start  = cc.p( -250, 700 ),
        goal   = cc.p( 610, 700 ),
        duration = 10,
        fire = { 3, bullet_player_high, 0.5, bullet_player_high, 0.5, bullet_player_high, 0.5, bullet_player_high }
    }
    local enemy_pattern410 = {
        update = moving.wait_once,
        start  = cc.p( 810, 800 ),
        goal   = cc.p( -50, 800 ),
        duration = 10,
        fire = { 3.7, bullet_player_high, 0.5, bullet_player_high, 0.5, bullet_player_high, 0.5, bullet_player_high }
    }
    local enemy_pattern500 = {
        update = moving.straight,
        start  = cc.p( 610, 800 ),
        goal   = cc.p( -50, 700 ),
        duration = 10,
        fire = { 3, bullet_homing, 3, bullet_homing }
    }
    local enemy_pattern510 = {
        update = moving.homing,
        start  = cc.p( -50, 600 ),
        goal   = cc.p( 610, 500 ),
        duration = 10,
        fire = { 3, bullet_homing, 3, bullet_homing },
    }
    local enemy_pattern600 = {
        update = moving.homing,
        start  = cc.p( -50, 600 ),
        goal   = cc.p( 610, 500 ),
        duration = 8,
        fire = { 2, bullet_player, 1, bullet_player, 1, bullet_player, 1, bullet_player, 1, bullet_player },
    }
    -------------- 

    object_layer:runAction(
        action.sequence({
            action.wait( 3 ),
            action.create_enemy( enemy_pattern000 ),
            action.wait( 4 ),
            action.create_enemy( enemy_pattern100 ),
            action.wait( 2 ),
            action.create_enemy( enemy_pattern100 ),
            action.wait( 4 ),
            action.create_enemy( enemy_pattern200 ),
            action.wait( 2 ),
            action.create_enemy( enemy_pattern210 ),
            action.wait( 2 ),
            action.create_enemy( enemy_pattern200 ),
            action.wait( 4 ),
            action.create_enemy( enemy_pattern200 ),
            action.create_enemy( enemy_pattern210 ),
            action.create_enemy( enemy_pattern300 ),
            action.wait( 6 ),
            action.create_enemy( enemy_pattern400 ),
            action.create_enemy( enemy_pattern410 ),
            action.wait( 4 ),
            action.create_enemy( enemy_pattern500 ),
            action.wait( 0.5 ),
            action.create_enemy( enemy_pattern500 ),
            action.wait( 0.5 ),
            action.create_enemy( enemy_pattern500 ),
            action.wait( 4 ),
            action.create_enemy( enemy_pattern510 ),
            action.wait( 0.5 ),
            action.create_enemy( enemy_pattern510 ),
            action.wait( 0.5 ),
            action.create_enemy( enemy_pattern510 ),
            action.wait( 4 ),
            action.create_enemy( enemy_pattern600 ),
            action.wait( 0.5 ),
            action.create_enemy( enemy_pattern600 ),
            action.wait( 0.5 ),
            action.create_enemy( enemy_pattern600 ),
            action.wait( 0.5 ),
            action.create_enemy( enemy_pattern600 ),
            action.wait( 0.5 ),
            action.create_enemy( enemy_pattern600 ),
            action.wait( 4 ),
            cc.CallFunc:create(function()
                c2xa.set_magnification( c2xa.get_magnification() + 0.1 )
                c2xa_generate( object_layer )
            end)
        })
    )
end