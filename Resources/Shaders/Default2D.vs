#version 150
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require


layout(location = 0)in vec2 position;
layout(location = 2)in vec2 texcoord;

out vec4 Color;
out vec2 Texcoord;

uniform mat4 model,proj;

void main()
{
    Texcoord = texcoord;
    gl_Position = proj*model*vec4(position,0.0f, 1.0 );

}
