
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

function moving.curve(start, line1, line2)
    return function(progress)
       return cc.p((math.cos(progress)) * progress / 100,(math.sin(progress)) * progress / 100)
       return cc.p((start.x * start.x) / (line1*line1))
    end
end

function moving.teleportation (start, goal, tyuuten)
    return function(progress)
       if progress < 30 then
            return cc.p(start.x,start.y)
       elseif progress < 70 then
            return cc.p(tyuuten.x,tyuuten.y)
	   else
            return cc.p(goal.x,goal.y)
       end
    end
end