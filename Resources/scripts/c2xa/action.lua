
require "c2xa.extend"
require "c2xa.moving"

action = action or {}

function action.create_enemy( setting )
    return cc.CallFunc:create(function()
        local enemy
        local default = {
            move = moving.default,
            time = 4.0,
            easing = nil -- óDêÊìxí·
        }
        setting = extend_deep( {}, default, setting )
        enemy = c2xa.create_enemy( setting )
        c2xa.enemies:addChild( enemy )
    end)
end

function action.create_bullet( setting )
    return cc.CallFunc:create(function()
        local bullet
        local default = {
            move = moving.default,
            time = 4.0,
            easing = nil -- óDêÊìxí·
        }
        setting = extend_deep( {}, default, setting )
        bullet = c2xa.create_bullet( setting )
        setting.parent:addChild( bullet )
    end)
end

function action.wait( time )
    return cc.DelayTime:create( time )
end

function action.call( func )
    return cc.CallFunc:create( func )
end

function action.sequence( seq )
    return cc.Sequence:create( seq )
end
