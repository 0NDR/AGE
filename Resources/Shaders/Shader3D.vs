#version 150

in vec3 position;
in vec3 normal;
in vec2 texcoord;
in vec3 tangent;
in vec3 bitangent;

out vec4 VertexPosition;
out vec2 Texcoord;
out vec3 Normal,VertexToEye,Tangent,Bitangent;
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
    Bitangent = NormalMatrix*bitangent;
    VertexPosition= (model*vec4(position, 1.0 ));
    VertexToEye = -vec3(view*VertexPosition);
    vec3 LightToVertex = vec3(LightPositions[0]-VertexPosition.xyz);
    color = ObjectColor;
    Texcoord = texcoord;
    gl_Position = proj*view*VertexPosition;

}
