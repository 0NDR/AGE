#version 150
#define pi 3.14159265659

const int lightMax =50;
in vec4 VertexPosition,color;
in vec2 texcoordo;
in vec3 normalo,tangento,texcoord30;

out vec4 outColor;
uniform sampler2D ObjectTexture;
uniform samplerCube CubeTexture;
uniform sampler2D Texture_Diffuse0;
uniform sampler2D Texture_Specular0;
uniform sampler2D Texture_Normal0,Texture_Height0;
uniform mat4 model, view, proj;
uniform vec3 scale,viewpos;
//uniform vec4 ObjectDiffuse, ObjectAmbient, ObjectSpecular, GlobalAmbient;
vec4 ObjectDiffuse=vec4(1,1,1,1);
vec4 ObjectAmbient=vec4(1,1,1,1);
vec4 ObjectSpecular=vec4(1,1,1,1);
vec4 GlobalAmbient=vec4(0,0,0,1);
int Shininess = 32;
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
uniform bool LightsOn, TextureOn, ColorOn, ZeroOn,isCube;

vec4 getLightsStupid()
{
    vec4 FINALLIGHT;
    vec3 n = normalize(normalo);
    for(int i=0;i<lightCount&&i<lightMax;++i)
    {
        vec3 LightToVertex = vec3(LightPositions[i]-VertexPosition.xyz);   //Surface to light
        float Diffuse =max(dot(n,LightToVertex),0);
        FINALLIGHT+=Diffuse;
    }
    return FINALLIGHT;

}
vec2 parallaxMapping(mat3 tbnMatrix,sampler2D Height,vec2 texcoord,vec2 scale)
{
    vec3 V = normalize(tbnMatrix*normalize(VertexPosition.xyz-viewpos));
   // determine number of layers from angle between V and N
   const float minLayers = 5;
   const float maxLayers = 15;
   float numLayers = 900;//mix(maxLayers, minLayers, abs(dot(vec3(0, 0, 1), V)));

   // height of each layer
   float layerHeight = 1.0 / numLayers;
   // depth of current layer
   float currentLayerHeight = 0;
   // shift of texture coordinates for each iteration
   vec2 dtex = scale.x * V.xy / V.z / numLayers;

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

vec4 GetLights(vec2 texcoord, sampler2D norm,mat3 tbnMatrix){

    vec3 bump = normalize(texture(norm, texcoord).xyz*2-1);    //unit vector in direction of the texture bump
    vec4 specularMap = texture(Texture_Specular0, texcoord);
    vec4 FINALLIGHT = GlobalAmbient*ObjectAmbient;

    for(int i=0;i<lightCount&&i<lightMax;++i){

        vec3 l = tbnMatrix*(LightPositions[i]-VertexPosition.xyz);   //vector with direction of light to vertex
        vec3 v = normalize(tbnMatrix*(VertexPosition.xyz-viewpos));
        float Distance = length(l);                            //distance between light and surface
        l = normalize(l);                          //make light to vertex vector a unit vector
        vec3 R = reflect(-l,bump);
        float nDotL=max(dot(bump,l),0);                  //cosine of the angle between the bump and the light to surface
        float Specular=0;
        if(nDotL!=0)
            Specular = pow(clamp(dot(R,v),0,1),Shininess);
        float SpotlightAngle = dot(normalize(LightDirections[i]),l); //Direction is the cos of the angle between the direction of the light and the vector pointing towards the vertex
        if(LightDirections[i]==vec3(0,0,0))
            SpotlightAngle =1;
        // COLORIZE LIGHTS
        if(SpotlightAngle>cos(radians(LightCutoffs[i]))){

            // CALCULATE ATTENUATION
            float AttenuationFinal = pow(SpotlightAngle,LightSpotExponents[i]);
            float AttenuationFactor =(LightConstantAttenuations[i]+LightLinearAttenuations[i]*Distance+LightQuadraticAttenuations[i]*Distance*Distance);
            if(AttenuationFactor!=0){
                AttenuationFinal = AttenuationFinal/AttenuationFactor;
            }
                FINALLIGHT += ObjectAmbient*LightAmbients[i];
                FINALLIGHT += AttenuationFinal*LightDiffuses[i]*ObjectDiffuse*nDotL*SpotlightAngle;
                FINALLIGHT += AttenuationFinal*LightSpeculars[i]*ObjectSpecular*Specular*SpotlightAngle;
       }
    }
    return vec4(FINALLIGHT.xyz,1);
}

void main() {

    outColor = vec4(1,1,1,1);
    vec3 n = normalize(normalo);                                     //unit Normal vector
    vec3 t = normalize(tangento);                                    //unit Tangent vector
    vec3 b = cross(n,t);                                            //cross them to make unit bitangent vector

    mat3 tbnMatrix = mat3(t,b,n);
    vec2 DisplacedTexCoord = texcoordo;
    //if(time%100>50)
    float amt = -time%100;
        DisplacedTexCoord = parallaxMapping(tbnMatrix,Texture_Normal0,texcoordo,vec2(.001,0));

    if(LightsOn)
       outColor *= GetLights(DisplacedTexCoord,Texture_Normal0,tbnMatrix);

    vec4 tex = texture(Texture_Normal0,DisplacedTexCoord);
    if(TextureOn)
    {
            if(isCube)
                outColor *= vec4(texture(CubeTexture,texcoord30).xyz,1);
            else
                outColor *= vec4(texture(Texture_Diffuse0,DisplacedTexCoord).xyz,1);
    }
    if(ColorOn)
        ;//outColor *= color;
    if(ZeroOn)
       ;// outColor *= vec4(0,0,0,1);
}
