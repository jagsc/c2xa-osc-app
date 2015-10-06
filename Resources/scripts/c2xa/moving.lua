
moving = moving or {}

-- デフォルトは移動しないので実質指定必須
function moving.default( progress )
    return cc.p( 100, 100 )
end

-- 直進
function moving.straight( start, goal )
    return function( progress )
        return cc.p(
            ( goal.x - start.x ) * progress / 100,
            ( goal.y - start.y ) * progress / 100 )
    end
end