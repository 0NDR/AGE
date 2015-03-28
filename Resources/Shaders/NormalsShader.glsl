#startVertex
#version 150
in vec3 position;
in vec3 normal;
in vec2 texcoord;
in vec3 tangent;
in vec3 bitangent;
in ivec4 BoneID;
in vec4 BoneWeight;


struct Vertex {
    vec4 modelPosition;
    vec4 worldPosition;
    vec2 Texcoord;
    vec3 Normal;
    vec3 Tangent;
    vec3 BiTangent;
    vec4 Color;
};

struct Matrices {
    mat4 model;
    mat4 view;
    mat4 proj;
    mat3 NormalMatrix;
    mat3 tbn; // converts from camera space to tangent space

};
out Vertex vOutput;
out Matrices vMat;
const int MAX_BONES = 100;
uniform bool HasBones;
uniform mat4 model, view, proj;
uniform mat4 Bones[MAX_BONES];
uniform vec4 ObjectColor;
uniform vec4 ObjectDiffuse, ObjectAmbient, ObjectSpecular;
uniform float time;
uniform vec3 LightPositions[50];
void main()
{
    mat4 BoneTransform=mat4(1.f);//mat4(1.0f);
    if(HasBones)
        {
        BoneTransform=mat4(0.f);//mat4(1.0f);
        for(int i=0;i<4;i++)
        {
            int id = BoneID[i];
            BoneTransform +=(Bones[id]*BoneWeight[i]);
        }
    }
    vMat.model=model*BoneTransform;
    vMat.view=view;
    vMat.proj=proj;
    vMat.NormalMatrix = mat3(transpose(inverse(vMat.model)));
    vOutput.Normal = normalize(vMat.NormalMatrix*normal);
    vOutput.Tangent = normalize(vMat.NormalMatrix*tangent);
    vOutput.BiTangent = normalize(vMat.NormalMatrix*bitangent);
    vMat.tbn = (mat3(vOutput.Tangent,vOutput.BiTangent,vOutput.Normal));

    vOutput.modelPosition= vec4(position,1);
    vOutput.worldPosition= (vMat.model*vOutput.modelPosition);
    vOutput.Color = vec4(1,1,1,1);
    vOutput.Texcoord = texcoord.xy;
    gl_Position = vOutput.modelPosition;

}

#endVertex




{
#startGeometry
#version 150
layout(triangles) in;

layout(line_strip, max_vertices=36) out;

float normal_length = 1;

struct Vertex {
    vec4 modelPosition;
    vec4 worldPosition;
    vec2 Texcoord;
    vec3 Normal;
    vec3 Tangent;
    vec3 BiTangent;
    vec4 Color;
};

struct Matrices {
    mat4 model;
    mat4 view;
    mat4 proj;
    mat3 NormalMatrix;
    mat3 tbn;

};
const int lightMax =50;
out vec3 color;
in Vertex vOutput[];
in Matrices vMat[];
void main()
{
  int i;
  for(i=0; i<gl_in.length(); i++)
  {
vec3 P = gl_in[i].gl_Position.xyz;
    vec3 N = vOutput[i].Normal.xyz;
    vec3 T = vOutput[i].Tangent.xyz;
    vec3 B = vOutput[i].BiTangent.xyz;
    color = vec3(0,0,1);
    gl_Position = vMat[i].proj*vMat[i].view*vMat[i].model * vec4(P, 1.0);
    EmitVertex();

    gl_Position = vMat[i].proj*vMat[i].view*vMat[i].model * vec4(P + T * normal_length, 1.0);
    EmitVertex();

    color = vec3(0,1,0);
    gl_Position = vMat[i].proj*vMat[i].view*vMat[i].model * vec4(P, 1.0);
    EmitVertex();

    gl_Position = vMat[i].proj*vMat[i].view*vMat[i].model * vec4(P + N * normal_length, 1.0);
    EmitVertex();

    color = vec3(1,0,0);
    gl_Position = vMat[i].proj*vMat[i].view*vMat[i].model * vec4(P, 1.0);
    EmitVertex();

    gl_Position = vMat[i].proj*vMat[i].view*vMat[i].model * vec4(P + B * normal_length, 1.0);
    EmitVertex();

    EndPrimitive();
  }
}
#endGeometry
}
{
#startFragment
#version 150
in vec3 color;
out vec4 outColor;

void main()
{
    outColor = vec4(color.xyz,1.f);


}
#endFragment
}

