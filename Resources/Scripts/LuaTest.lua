clocktime = Clock()
if(clocktime%1==0)then
    a = Instance:new("gameObject")
    a.Parent = Floor
    a.Position = vec3(math.random(-1000,1000)/100,.4,math.random(-1000,1000)/100)
end
children = Floor:getChildren()
child = children[1]
theta = clocktime/1000
r = .5
child.Position = vec3(math.cos(theta),.5,math.sin(theta))*r
--child.Rotation = vec3(90,-(theta/(math.pi))*180,0)
