function main()
    children = Floor:getChildren()
    clocktime = Clock()
    for i=1,30 do
        child = children[i]
        theta = (2*math.pi)*(i/100)+clocktime/1000
        r = ((i+1)/75)
        child.Position = vec3(math.cos(theta),.5,math.sin(theta))*r
        child.Rotation = vec3(90,-(theta/(math.pi))*180,0)
    end
end
main()
