require "c2xa.action"

-- 進捗率(0~100)を受け取り、現在存在するべき座標を返す
function straight( start, goal )
    return function( progress )
        return cc.p( ( goal.x - start.x ) * progress / 100, ( goal.y - start.y ) * progress / 100 )
    end
end

-- 1クール
function c2xa_generate( node )
    local straight_sample = straight( cc.p( 0, 0 ), cc.p( 560, 960 ) )
    node:runAction(
        action.sequence({
            action.create_enemy({ enemies = node, move = straight_sample }),
            action.wait( 2 ),
            action.create_enemy({ enemies = node, move = straight_sample }),
            action.wait( 2 ),
            action.create_enemy({ enemies = node, move = straight_sample }),
            action.wait( 2 ),
            action.create_enemy({ enemies = node, move = straight_sample }),
            action.wait( 2 ),
            action.create_enemy({ enemies = node, move = straight_sample })
        })
    )
end