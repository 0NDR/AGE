#version 150

in vec3 position;
in vec3 normal;
in vec2 texcoord;
in vec3 tangent;
in vec3 bitangent;

out vec4 VertexPosition;
out vec2 Texcoord;
out vec3 Normal,Tangent,Texcoord3d;
out vec4 color;

uniform mat4 model, view, proj;
uniform vec3 translation;
uniform vec4 ObjectColor;
uniform vec4 ObjectDiffuse, ObjectAmbient, ObjectSpecular;
uniform float time;
uniform vec3 LightPositions[50];
void main()
{
     mat3 NormalMatrix = mat3(transpose(inverse(model)));
    Normal = NormalMatrix *normal;
    Tangent = NormalMatrix*tangent;
    VertexPosition= (model*vec4(position, 1.0 ));
    color = ObjectColor;
    Texcoord = texcoord;
    Texcoord3d = position;
    gl_Position = proj*view*VertexPosition;

}
