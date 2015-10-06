
--[[
c2xa = c2xa or {}

c2xa.enemy = class("c2xa.enemy",function()
    return cc.Node:create()
end)

function c2xa.enemy:ctor( setting )
    self.move = setting.move
    self.scheduler_id = setting.scheduler_id
    self.progress = 0
    self.sprite = cc.Sprite:create( "img/player_bugdroid.png" )
    self.sprite:setPosition( cc.p( 0, 0 ) )
    self:addChild( self.sprite )
end

function c2xa.enemy:update( delta )
    self.progress = self.progress + delta * 100
    if( self.progress > 100 ) then
        cc.Director:getInstance():getScheduler():unscheduleScriptEntry( self.scheduler_id )
        self:removeAllChildren()
        self:removeFromParent()
        return
    end
    self.sprite:setPosition( self.move( cc.p( 0, 0 ), cc.p( 560, 960 ), self.progress ) )
end
]]