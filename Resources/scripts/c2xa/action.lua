require "c2xa.enemy"

action = action or {}

function action.create_enemy( setting )
    return cc.CallFunc:create(function()
        local enemy
        setting.scheduler_id = cc.Director:getInstance():getScheduler():scheduleScriptFunc( function( delta )
            enemy:update( delta )
        end, 0, false )
        enemy = c2xa.enemy.new( setting )
        setting.enemies:addChild( enemy )
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
