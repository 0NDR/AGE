#version 150

out vec4 VertexPosition;
out vec2 Texcoord;
out vec2 lightPositionOnScreen;

in vec3 position;
in vec2 texcoord;

uniform mat4 model;
uniform vec2 lPosition;

vec4 rotFactor;
vec4 offFactor;
void main()
{
    Texcoord = texcoord;
    rotFactor = vec4(.02,-.02,0,1);
    offFactor = vec4(-1,1,0,0);
    vec4 vert =  model*vec4(position, 1.0 );
    VertexPosition=vert;//*rotFactor+offFactor;
    gl_Position =VertexPosition;

}
