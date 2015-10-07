
function extend_deep( to, from, ... )
    for key, value in pairs( from ) do
        if type( value ) == "table" then
            to[ key ] = {}
            extend_deep( to[ key ], value )
        else
            to[ key ] = value
        end
    end
    if( select( "#", ... ) > 0 ) then
        return extend_deep( to, ... )
    else
        return to
    end
end