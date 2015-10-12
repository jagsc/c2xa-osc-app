
require "c2xa.extend"
require "c2xa.moving"

action = action or {}

function action.create_enemy( setting )
    return cc.CallFunc:create(function()
        local enemy
        local default = {
            duration = 10.0,
            point = 1000,
            fire = nil,
        }
        setting = extend_deep( {}, default, setting )
        enemy = c2xa.create_enemy( setting )
        c2xa.enemies:addChild( enemy )
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
