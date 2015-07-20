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
    vec4 finalPosition;
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
    vOutput.finalPosition = vMat.proj*vOutput.cameraPosition;//proj*view*vOutput.worldPosition;
    gl_Position=vOutput.finalPosition;
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
    vec4 cameraPosition;
    vec4 finalPosition;
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
    vec4 finalPosition;
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
out vec4 outColor;
out float outColor1;
uniform sampler2D ObjectTexture;
uniform sampler2D Texture_Diffuse0,Texture_Specular0,Texture_Normal0,Texture_Height0;
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

vec3 blend(in vec4 texture1, in float a1, in vec4 texture2,in float a2)
{
    float depth = 0.2;
    float ma = max(texture1.a + a1, texture2.a + a2) - depth;

    float b1 = max(texture1.a + a1 - ma, 0);
    float b2 = max(texture2.a + a2 - ma, 0);

    return vec3((texture1.rgb * b1 + texture2.rgb * b2) / (b1 + b2));
}
void main() {


    vec4 FinalColor = ObjectColor;
    if(FinalColor == vec4(0,0,0,0))
    {
        FinalColor = vec4(1,1,1,1);
    }
    ivec2 frames = ivec2(12,6);
    vec2 dist = vec2(85.0,85.0);
    vec2 reso = dist*frames;
    int frametime=50;
    int frame = (time/frametime)%(frames.x*frames.y);
    vec2 framedisp = vec2(frame%frames.x,frame/frames.y)*dist/reso;
    vec2 DisplacedTexCoord = (vOutput.Texcoord)*TextureScaling+framedisp;
    if(LightsOn)
    {
        if(NormalOn)
        {
            FinalColor *= getLightsStupid();
        }
        else
        {

            FinalColor *= getLights(DisplacedTexCoord,Texture_Normal0,vMat.tbn);
        }
    }
    if(TextureOn)
    {
        if(isCube)
        {
            //FinalColor *= vec4(textureCube(CubeTexture,vOutput.modelPosition.xyz).xyz,1.f);
        }
        else
        {

                FinalColor *=texture2D(Texture_Diffuse0,DisplacedTexCoord);
        }
    }

    float z = gl_FragCoord.z;//vOutput.cameraPosition.z;
    outColor = vec4(FinalColor.r,FinalColor.g,FinalColor.b,FinalColor.a);
    if(transparency)
    {
         float weight =
                    max(min(1.0, max(max(FinalColor.r, FinalColor.g), FinalColor.b) * FinalColor.a), FinalColor.a) *
                    max(0.03 / (1e-5 + abs(pow(z / 200, 4.0))), 1e-4);

        outColor =vec4(FinalColor.rgb*FinalColor.a,FinalColor.a)*weight/5000.0;
        outColor1 = FinalColor.a;

    }
    //if(ColorOn)
   /* if(outColor.a<.5 && ALPHADIS)
        discard;*/
}
#endFragment
}
/*    float z = -vOutput.cameraPosition.z;
    outColor = vec4(FinalColor.r,FinalColor.g,FinalColor.b,FinalColor.a);
    if(transparency)
    {
         float weight =pow(FinalColor.a + 0.01f, 4.0f) +
                   max(0.01f, min(3000.0f, 0.3f / (0.00001f + pow(abs(z) / 200.0f, 4.0f))));

        outColor =vec4(FinalColor.rgb*FinalColor.a*weight,FinalColor.a);
        outColor1 = vec4(FinalColor.a)*weight;
*/
