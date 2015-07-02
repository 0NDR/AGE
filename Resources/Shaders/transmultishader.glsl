#startVertex
#version 330 core
#extension GL_ARB_explicit_attrib_location : enable
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in ivec4 BoneID;
layout(location = 6) in vec4 BoneWeight;


struct Vertex {
    vec4 modelPosition;
    vec4 worldPosition;
    vec4 cameraPosition;
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
    mat3 tbn; // converts from world space to tangent space

};
out Vertex vOutput;
out Matrices vMat;
const int MAX_BONES = 100;
uniform bool HasBones;
uniform mat4 model, view, proj;
uniform mat4 Bones[MAX_BONES];
uniform vec4 ObjectColor;
uniform vec4 ObjectDiffuse, ObjectAmbient, ObjectSpecular;
uniform int time;
uniform sampler2D Texture_Diffuse0,grassHeight,gtext,grassTexture,Texture_Specular0,Texture_Normal0,Texture_Height0;

uniform vec3 LightPositions[3];
void main()
{
    mat4 BoneTransform=mat4(0.f);//mat4(1.0f);
    for(int i=0;i<4;i++)
    {
        int id = BoneID[i];
        BoneTransform +=(Bones[id]*BoneWeight[i]);
    }
    if(!HasBones)
    {
        BoneTransform = mat4(1.f);
    }
    vMat.model=model*BoneTransform;
    vMat.view=view;
    vMat.proj=proj;
    mat3 mMat3 = mat3(vMat.model);
    vMat.NormalMatrix = transpose(inverse(mMat3));
    vOutput.Normal = normalize(vMat.NormalMatrix*normal);
    vOutput.Tangent = normalize(mMat3*tangent);
    vOutput.BiTangent = normalize(mMat3*bitangent);
    vMat.tbn = mat3(vOutput.Tangent,vOutput.BiTangent,vOutput.Normal);
    vOutput.modelPosition= vec4(position,1);
    vOutput.worldPosition= (vMat.model*vOutput.modelPosition);
    vOutput.cameraPosition= (vMat.view*vOutput.worldPosition);
    vOutput.Color = vec4(1,1,1,1);
    vOutput.Texcoord = texcoord.xy;
    gl_Position = vMat.proj*vOutput.cameraPosition;//proj*view*vOutput.worldPosition;

}

#endVertex




{
#startGe1ometry
#version 330 core
layout(triangles) in;

layout(line_strip, max_vertices=6) out;

float normal_length = .2;

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
    mat3 normal;
    mat3 tbn;

};
const int lightMax =50;
out vec3 coloro;
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
    coloro = vec3(0,0,1);
    gl_Position = vMat[i].proj*vMat[i].view*vMat[i].model * vec4(P, 1.0);
    EmitVertex();

    gl_Position = vMat[i].proj*vMat[i].view*vMat[i].model * vec4(P + T * normal_length, 1.0);
    EmitVertex();
    coloro = vec3(0,1,0);
    gl_Position = vMat[i].proj*vMat[i].view*vMat[i].model * vec4(P, 1.0);
    EmitVertex();

    gl_Position = vMat[i].proj*vMat[i].view*vMat[i].model * vec4(P + N * normal_length, 1.0);
    EmitVertex();

    coloro = vec3(1,0,0);
    gl_Position = vMat[i].proj*vMat[i].view*vMat[i].model * vec4(P, 1.0);
    EmitVertex();

    gl_Position = vMat[i].proj*vMat[i].view*vMat[i].model * vec4(P + B * normal_length, 1.0);
    EmitVertex();

    EndPrimitive();
  }
}
#endGeome1try
}
{
#startFragment
#version 330 core
#define pi 3.14159265659
struct Vertex {
    vec4 modelPosition;
    vec4 worldPosition;
    vec4 cameraPosition;
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
const int lightMax =3;
in Vertex vOutput;
in Matrices vMat;
out vec4 outColor,outColor1;
uniform sampler2D ObjectTexture;
uniform samplerCube CubeTexture;
uniform sampler2D Texture_Diffuse0,grassHeight,gtext,grassTexture,Texture_Specular0,Texture_Normal0,Texture_Height0;
uniform vec3 viewpos;
uniform vec2 TextureScaling;
uniform vec4 ObjectColor;
//uniform vec4 ObjectDiffuse, ObjectAmbient, ObjectSpecular, GlobalAmbient;
vec4 ObjectDiffuse=vec4(1,1,1,1);
vec4 ObjectAmbient=vec4(1,1,1,1);
vec4 ObjectSpecular=vec4(1,1,1,1);
vec4 GlobalAmbient=vec4(0,0,0,1);
int Shininess = 8;
uniform float LightCutoffs[lightMax];
uniform float LightSpotExponents[lightMax];
uniform vec4 LightAmbients[lightMax];
uniform vec4 LightDiffuses[lightMax];
uniform vec4 LightSpeculars[lightMax];
uniform vec3 LightPositions[lightMax];
uniform vec3 LightDirections[lightMax];
uniform float LightLinearAttenuations[lightMax];
uniform float LightConstantAttenuations[lightMax];
uniform float LightQuadraticAttenuations[lightMax];
uniform int lightCount;
uniform int time;
uniform int Unicolors;
uniform bool LightsOn, TextureOn, ColorOn, ZeroOn,isCube,NormalOn,grass, transparency;

void main() {

    vec4 tcolor = ObjectColor;
    float z = length(vOutput.cameraPosition);
    if(transparency)
    {
        float weight =
                    max(min(1.0, max(max(tcolor.r, tcolor.g), tcolor.b) * tcolor.a), tcolor.a) *
                    clamp(0.03 / (1e-5 + pow(z / 200, 4.0)), 1e-2, 3e3);
        outColor =vec4(tcolor.rgb*tcolor.a,tcolor.a)*weight/500;
        outColor1 = vec4(tcolor.a);


    }

}
#endFragment
}

