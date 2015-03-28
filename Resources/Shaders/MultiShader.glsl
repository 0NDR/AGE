#startVertex
#version 150
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
#version 150
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
#version 150
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
const int lightMax =50;
in Vertex vOutput;
in Matrices vMat;
out vec4 outColor;
uniform sampler2D ObjectTexture;
uniform samplerCube CubeTexture;
uniform sampler2D Texture_Diffuse0;
uniform sampler2D Texture_Specular0;
uniform sampler2D Texture_Normal0,Texture_Height0;
uniform vec3 viewpos;
uniform vec2 TextureScaling;
//uniform vec4 ObjectDiffuse, ObjectAmbient, ObjectSpecular, GlobalAmbient;
vec4 ObjectDiffuse=vec4(1,1,1,1);
vec4 ObjectAmbient=vec4(1,1,1,1);
vec4 ObjectSpecular=vec4(1,1,1,1);
vec4 GlobalAmbient=vec4(0,0,0,1);
int Shininess = 256;
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
uniform bool LightsOn, TextureOn, ColorOn, ZeroOn,isCube,NormalOn;

vec4 getLightsStupid()
{
    vec4 FINALLIGHT;
    vec3 n = normalize(vOutput.Normal);
    for(int i=0;i<lightCount&&i<lightMax;++i)
    {
        vec3 LightToVertex = vec3(LightPositions[i].xyz-vOutput.worldPosition.xyz);   //Surface to light
        float Distance = length(LightToVertex);
        LightToVertex = normalize(LightToVertex);
        vec3 R = reflect(-LightToVertex,n);
        vec3 v = normalize(viewpos-vOutput.worldPosition.xyz);
        float Diffuse =max(dot(n,normalize(LightToVertex)),0);
        float Specular = pow(clamp(dot(R,v),0,1),Shininess);
        float AttenuationFactor =(LightConstantAttenuations[i]+LightLinearAttenuations[i]*Distance+LightQuadraticAttenuations[i]*Distance*Distance);
        if(AttenuationFactor==0)
            AttenuationFactor=1.f;
        FINALLIGHT+=LightAmbients[i];
        FINALLIGHT+=LightDiffuses[i]*Diffuse/AttenuationFactor;
        FINALLIGHT+=LightSpeculars[i]*Specular/AttenuationFactor;

    }
    return vec4(FINALLIGHT.xyz,1);

}
vec2 parallaxMapping(mat3 tbnMatrix,sampler2D Height,vec2 texcoord,vec2 scaler)
{
    vec3 V = normalize(tbnMatrix*(viewpos-vOutput.worldPosition.xyz));
   // determine number of layers from angle between V and N
   const float minLayers = 5.f;
   const float maxLayers = 15.f;
   float numLayers = 900.f;//mix(maxLayers, minLayers, abs(dot(vec3(0, 0, 1), V)));

   // height of each layer
   float layerHeight = 1.0f / numLayers;
   // depth of current layer
   float currentLayerHeight = 0;
   // shift of texture coordinates for each iteration
   vec2 dtex = scaler.x * V.xy / V.z / numLayers;

   // current texture coordinates
   vec2 currentTextureCoords = texcoord;

   // get first depth from heightmap
   float heightFromTexture = 1-(texture2D(Height, currentTextureCoords).a);

   // while point is above surface
   while(heightFromTexture > currentLayerHeight)
   {
      // to the next layer
      currentLayerHeight += layerHeight;
      // shift texture coordinates along vector V
      currentTextureCoords -= dtex;
      // get new depth from heightmap
      heightFromTexture = 1-(texture2D(Height, currentTextureCoords).a);
   }

   // return results
   return currentTextureCoords;
}

vec4 getLights(vec2 texcoord, sampler2D norm,mat3 tbnMatrix){

    vec3 bump = normalize((tbnMatrix*(texture2D(norm, texcoord).xyz*2-1)).xyz);    //get the bump normal from the texture, convert it from tangent to world space, then adjust for non-uniform scale.
    vec4 specularMap = texture2D(Texture_Specular0, texcoord);
    vec4 FINALLIGHT = GlobalAmbient*ObjectAmbient;
    for(int i=0;i<lightCount&&i<lightMax;++i){

        vec3 l = (LightPositions[i].xyz-vOutput.worldPosition.xyz);   //vector with direction of light to vertex
        vec3 v = (viewpos-vOutput.worldPosition.xyz);
        float Distance = length(l);                            //distance between light and surface
        l = normalize(l);                          //make light to vertex vector a unit vector
        v = normalize(v);
        vec3 R = reflect(-l,bump);
        float Diffuse=max(dot(bump,l),0);                  //cosine of the angle between the bump and the light to surface
        float Specular=0.f;
        if(Diffuse!=0)
            Specular = pow(clamp(dot(R,v),0,1),Shininess);
        float SpotlightAngle = 1.f;
        if(LightDirections[i]!=vec3(0,0,0))//Direction is the cos of the angle between the direction of the light and the vector pointing towards the vertex

        {
                   SpotlightAngle = dot(normalize(LightDirections[i]),normalize(vOutput.worldPosition.xyz-LightPositions[i]));
        }
        // COLORIZE LIGHTS
        if(SpotlightAngle>LightCutoffs[i]){

            // CALCULATE ATTENUATION
            float AttenuationFinal = pow(SpotlightAngle,LightSpotExponents[i]);
            float AttenuationFactor =(LightConstantAttenuations[i]+LightLinearAttenuations[i]*Distance+LightQuadraticAttenuations[i]*Distance*Distance);
            if(AttenuationFactor>0){
                AttenuationFinal = AttenuationFinal/AttenuationFactor;
            }
                FINALLIGHT += ObjectAmbient*LightAmbients[i];
                FINALLIGHT += LightDiffuses[i]*ObjectDiffuse*Diffuse*AttenuationFinal;
                FINALLIGHT += LightSpeculars[i]*ObjectSpecular*Specular*AttenuationFinal;
       }
    }
    return vec4(FINALLIGHT.xyz,1.f);
}

void main() {

    outColor = vec4(1.f,1.f,1.f,1.f);

    vec2 DisplacedTexCoord = vOutput.Texcoord*TextureScaling;
    //DisplacedTexCoord = parallaxMapping(vMat.tbn,Texture_Normal0,DisplacedTexCoord,vec2(.001,0));

    if(LightsOn)
    {
        if(NormalOn)
        {
            outColor *= getLightsStupid();
        }
        else
        {
            outColor *= getLights(DisplacedTexCoord,Texture_Normal0,vMat.tbn);
        }
    }
    if(TextureOn)
    {
        if(isCube)
        {
            //outColor *= vec4(textureCube(CubeTexture,vOutput.modelPosition.xyz).xyz,1.f);
        }
        else
        {
            outColor *= vec4(texture2D(Texture_Diffuse0,DisplacedTexCoord).xyz,1.f);
        }
    }
    //if(ColorOn)
    //outColor = vec4(1,1,1,1);
}
#endFragment
}

