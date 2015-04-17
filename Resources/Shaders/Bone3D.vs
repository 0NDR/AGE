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
uniform float time;
uniform vec3 LightPositions[50];
void main()
{
    mat4 BoneTransform=mat4(0.f);//mat4(1.0f);
    for(int i=0;i<4;i++)
    {
        int id = BoneID[i];
        BoneTransform +=(Bones[id]*BoneWeight[i]);
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
