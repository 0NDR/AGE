#version 150

out vec4 VertexPosition;
out vec2 Texcoord;
out vec4 color;

in vec3 position;
in vec2 texcoord;

uniform vec4 ObjectColor;
uniform mat4 model, view, proj;

void main()
{
    Texcoord = texcoord;

    vec3 Center = vec3(model[3][0],model[3][1],model[3][2]);
    vec3 size = vec3(length(vec3(model[0][0],model[0][1],model[0][2])),length(vec3(model[1][0],model[1][1],model[1][2])),length(vec3(model[2][0],model[2][1],model[2][2])));
    color = ObjectColor;
    gl_Position =proj*view*vec4(Center,1.0);
    gl_Position /= gl_Position.w;
    gl_Position.xy += position.xy*size.xy;

}
