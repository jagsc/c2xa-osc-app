require "c2xa.action"

-- 進捗率(0~100)を受け取り、現在存在するべき座標を返す
function straight( start, goal )
    return function( progress )
        return cc.p( ( goal.x - start.x ) * progress / 100, ( goal.y - start.y ) * progress / 100 )
    end
end

function curve(start, line1, line2)
    return function(progress)
       return cc.p((math.cos(progress)) * progress / 100,(math.sin(progress)) * progress / 100)
       return cc.p((start.x * start.x) / (line1*line1))
    end
end

function teleportation (start, goal, tyuuten)
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

-- 1クール
function c2xa_generate( node )
    local straight_sample = straight( cc.p( 0, 0 ), cc.p( 560, 960 ) )
    local curve_sample = curve( cc.p( 300, 300 ), 100, 30 )    
	local teleportation_sample = teleportation(cc.p( 0, 0 ), cc.p( 200, 480 ), cc.p(480, 200 ))
    node:runAction(
        action.sequence({
            action.create_enemy({ enemies = node, move = straight_sample }),
            action.wait( 2 ),
            action.create_enemy({ enemies = node, move = straight_sample }),
            action.wait( 2 ),
            action.create_enemy({ enemies = node, move = curve_sample }),
            action.wait( 2 ),
            action.create_enemy({ enemies = node, move = teleportation_sample }),
            action.wait( 2 ),
            action.create_enemy({ enemies = node, move = straight_sample })
        })
    )
end