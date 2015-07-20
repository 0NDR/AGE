function main()
    children = Floor:getChildren()
    clocktime = Clock()
    for i=1,36 do
        child = children[i]
        theta = (2*math.pi)*(i/36)+clocktime/1000
        r = .5--((i+1)/25)
        child.Position = vec3(math.cos(theta),.5,math.sin(theta))*r
        --child.Rotation = vec3(90,-(theta/(math.pi))*180,0)
    end
end
main()
