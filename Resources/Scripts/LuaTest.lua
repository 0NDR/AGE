clocktime = Clock()
children = Floor:getChildren()
if(#children<50)then
    math.randomseed(Clock())
    a = Instance:new("gameObject",Floor)
    theta = (math.random(0,1000)/1000)*math.pi*2
    r= math.random(0,1000)/25
    a.Position = vec3(r*math.cos(theta),1,r*math.sin(theta))
    a.Rotation = vec3(0,math.random(0,3600)/10,0)
end
--child.Rotation = vec3(90,-(theta/(math.pi))*180,0)
