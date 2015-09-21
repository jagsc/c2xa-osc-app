function class(classname, super)
    local superType = type(super)
    local cls

    if superType ~= "function" and superType ~= "table" then
        superType = nil
        super = nil
    end

    if superType == "function" or (super and super.__ctype == 1) then
        -- inherited from native C++ Object
        cls = {}

        if superType == "table" then
            -- copy fields from super
            for k,v in pairs(super) do cls[k] = v end
            cls.__create = super.__create
            cls.super    = super
        else
            cls.__create = super
        end

        cls.ctor    = function() end
        cls.__cname = classname
        cls.__ctype = 1

        function cls.new(...)
            local instance = cls.__create(...)
            -- copy fields from class to native object
            for k,v in pairs(cls) do instance[k] = v end
            instance.class = cls
            instance:ctor(...)
            return instance
        end

    else
        -- inherited from Lua Object
        if super then
            cls = clone(super)
            cls.super = super
        else
            cls = {ctor = function() end}
        end

        cls.__cname = classname
        cls.__ctype = 2 -- lua
        cls.__index = cls

        function cls.new(...)
            local instance = setmetatable({}, cls)
            instance.class = cls
            instance:ctor(...)
            return instance
        end
    end

    return cls
end

require "lua/Cocos2dx/cocos2d/Cocos2d.lua"

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