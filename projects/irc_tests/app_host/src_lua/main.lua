
require( "mcuctrl" )

function main()
    local client = nil
    local timeoutToConnect = 10
    local timeToGetClient  = 180
    local triesLeft = 3
    local connected
    for ttt = 1, triesLeft do
        local t0 = os.time()
        local t1 = t0
        while ( t1 - t0 < timeoutToConnect ) do
            t1 = os.time()
            msleep( 1 )
            local connected = isConnected()
            if ( connected ) then
                break
            end
        end
        if ( connected ) then
            break
        end
    end
    -- Connection with server established.
    if ( connected ) then
        send( "print( \'Online!\' )" )
        t0 = os.time()
        t1 = t0
        while ( t1 - t0 < timeToClient ) do
            t1 = os.time()
            msleep( 1 )
            -- If there was timer reset by client
            -- the "client" variable is set.
            -- Then reset timeout to shutdown.
            if ( client ) then
                t0 = t1
                client = nil
            end
        end
    end
    ps = luaprocess.create()
    ps:start( "halt", "-p" )
    ps:waitForFinished()
end

main()






