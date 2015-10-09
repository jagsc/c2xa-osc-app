
moving = moving or {}

-- デフォルトは移動しないので実質指定必須
function moving.default( progress )
    return cc.p( 100, 100 )
end

-- 直進
function moving.straight( start, goal )
    return function( progress )
        return cc.p(
            start.x + ( goal.x - start.x ) * progress / 100,
            start.y + ( goal.y - start.y ) * progress / 100 )
    end
end

function moving.curve(start, goal, tyuuten)
    return function(progress)	   
       if progress < 50 then
            return cc.p( start.x + ( tyuuten.x - start.x ) * (progress*2) / 100, start.y + ( tyuuten.y - start.y ) * (progress*2) / 100)
       else
            return cc.p( tyuuten.x + ( goal.x - tyuuten.x ) * (progress) / 100, tyuuten.y + ( goal.y - tyuuten.y ) * (progress) / 100)        
       end
    end
end

function moving.teleportation (start, goal, tyuuten)
    return function(progress)
       if progress < 30 then
            return cc.p(start.x,start.y)
       elseif progress < 50 then
            return cc.p(tyuuten.x,tyuuten.y)
	   else
            return cc.p(goal.x,goal.y)
       end
    end
end