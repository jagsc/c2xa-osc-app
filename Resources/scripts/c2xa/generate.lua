require "c2xa.action"

-- スタート地点、ゴール地点、進捗率(0~100)を受け取り、現在存在するべき座標を返す
function curve( start, goal, progress )
    -- カーブって名前だけどとりあえず直線等速
    return cc.p( ( goal.x - start.x ) * progress / 100, ( goal.y - start.y ) * progress / 100 )
end

-- 1クール
function c2xa_generate( node )
    node:runAction(
        action.sequence({
            action.create_enemy({ enemies = node, move = curve }),
            action.wait( 2 ),
            action.create_enemy({ enemies = node, move = curve }),
            action.wait( 2 ),
            action.create_enemy({ enemies = node, move = curve }),
            action.wait( 2 ),
            action.create_enemy({ enemies = node, move = curve }),
            action.wait( 2 ),
            action.create_enemy({ enemies = node, move = curve })
        })
    )
end