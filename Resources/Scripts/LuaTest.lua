pi = 3.1415926
kek = 0
imgindex = 0
function newscript(par,m)
    a = vec3(math.random(0,1),math.random(0,1),math.random(0,1))
    go = K.IF:new("UI")
    go.Name = "thisguy"..kek
    go:setMesh(K.smesh)
    --go:setShader(K.shader)
    go.Color = vec4(1,1,1,1)
    go.Position = vec4(0,0,0,0)
    go.Size = vec4(0,200,0,200)
    go.Parent = par
    return go
end
--newscript(game2d,0)
Goy = newscript(game2d,0)

Goy.Size = vec4(0,10,0,50)
Goy.Position = vec4(-1,Goy.Size.y,1,-Goy.Size.w)
Goy.Color = vec4(0,0,0,1)
indexPosition = 0
function onkey(key)
    kek=kek+1
    if key.key.keysym.sym==KEY_LSHIFT then
        imgindex=imgindex+1
        K.texture:saveImageToFile("C:/Users/Nick/Dropbox/Apps/AGE/bin/release/img"..imgindex..".png")
    elseif key.key.keysym.sym==KEY_BACKSPACE and indexPosition>0 then
        K.texture:setText(string.sub(K.texture:getText(),0,string.len(K.texture:getText())-1))
        K.texture:drawText()
        K.texture:loadText(true)
        indexPosition=indexPosition-1
    elseif key.key.keysym.sym<250 then
        indexPosition=indexPosition+1
        addonstring = string.char(key.key.keysym.sym)
        if K.Controller:getModState(KMOD_SHIFT) or K.Controller:getModState(KMOD_CAPS) then
            addonstring = string.upper(addonstring)
        end
        K.texture:setText(K.texture:getText()..addonstring)
        K.texture:drawText()
        K.texture:loadText(true)
    end
    GoyPos=K.texture:getTextLength(-1)
    Goy.Size = vec4(0,10,0,GoyPos.y/2)
    Goy.Position = vec4(-1,Goy.Size.y+GoyPos.x,1,-Goy.Size.w)

end


K.Controller:addEvent(onkey,EVENT_KeyDown)
while true do
    Goy.Color.a = 1-Goy.Color.a
    wait(300)
end

