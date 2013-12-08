luaMumbleLink
=============

lua module for positional audio

The C code is from this example: http://mumble.sourceforge.net/Link


This is nothing great, especially not yet.


Build:
    Linux: make
    Other OSs: not tested

Use:
    require "mumblelink"
    
    function yourInitFunction()
        initMumble()
    end
    
    function yourUpdateFunction()
        updateMumble( identity, context, posx, posy, camx, camy )
        --[[
           identity: player id/name, string
           context: server address or similar, string
           posx, posy: 2D position of player, float
           camx, camy: 2D postiion of the camera, float
        ]]
    end

Linking to mumble works, position not tested.
