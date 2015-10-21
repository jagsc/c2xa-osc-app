
local bullet = class("bullet",function()
    return cc.Node:create()
end)

--This function like the construtor of c++ class
function bullet:ctor()
    -- do initialized
end

a = cc.p(0,0)

function helloLua(scene)
    local bullets_node_ = scene:getChildByName( "object_layer" ):getChildByName( "enemy_bullets" )
    local label = cc.LabelTTF:create("Hello Lua!", "Thonburi", 34)
    label:setPosition(scene:getContentSize().width/2, scene:getContentSize().height/2)
    local node_ = bullet.new()
    node_:addChild(label)
    bullets_node_:addChild(node_)
    local cnt_ = 0
    function update(d_)
        cnt_ = cnt_ + ( d_ * 100 ) / 20
        label:setPosition(cc.p(math.cos(cnt_) * 100 + 100, math.sin(cnt_) * 100 + 100))
    end
    local scheduler = cc.Director:getInstance():getScheduler()
    local schedulerID = scheduler:scheduleScriptFunc(update, 0, false)
end