#version 150
#define pi 3.14159265659

const int lightMax =50;
in vec4 VertexPosition;
in vec2 Texcoord;
in vec3 Normal,VertexToEye,Bitangent,Tangent;

out vec4 outColor;
uniform sampler2D ObjectTexture;
uniform sampler2D NormalMap;
uniform mat4 model, view, proj;
uniform vec3 scale;
//uniform vec4 ObjectDiffuse, ObjectAmbient, ObjectSpecular, GlobalAmbient;
vec4 ObjectDiffuse=vec4(1,1,1,1);
vec4 ObjectAmbient=vec4(1,1,1,1);
vec4 ObjectSpecular=vec4(1,1,1,1);
vec4 GlobalAmbient=vec4(0,0,0,1);
int Shininess = 256;
uniform float LightCutoffs[lightMax];
uniform float LightSpotExponents[lightMax];
in vec4 color;
uniform vec4 LightAmbients[lightMax];
uniform vec4 LightDiffuses[lightMax];
uniform vec4 LightSpeculars[lightMax];
uniform vec3 LightPositions[lightMax];
uniform vec3 LightDirections[lightMax];
uniform float LightLinearAttenuations[lightMax];
uniform float LightConstantAttenuations[lightMax];
uniform float LightQuadraticAttenuations[lightMax];
uniform int lightCount;
uniform float time;
uniform int Unicolors;
uniform bool LightsOn, TextureOn, ColorOn, ZeroOn;

vec4 getColorFromAlt(float altitude)
{
    if(altitude<=0.05)
        return vec4(0,0,1,1);
    else if(altitude>0.05 && altitude<=.1)
        return vec4(.5,.5,1,1);
    else if(altitude>0.1 && altitude<=.2)
        return vec4(1,1,0,1);
    else if(altitude>.2&&altitude<=.50)
        return vec4(0,1,0,1);
    else if(altitude>.5&&altitude<=.68)
        return vec4(.65,.16,.16,1);
    else if(altitude>.68&&altitude<=.85)
        return vec4(.5,.5,.5,1);
    else if(altitude>.85)
        return vec4(1,1,1,1);
}
vec4 getLightsStupid()
{
    vec4 FINALLIGHT;
    vec3 n = normalize(Normal);
    for(int i=0;i<lightCount&&i<lightMax;++i)
    {
        vec3 LightToVertex = vec3(LightPositions[i]-VertexPosition.xyz);   //Surface to light
        float Diffuse =max(dot(n,LightToVertex),0);
        FINALLIGHT+=Diffuse;
    }
    return FINALLIGHT;

}
vec4 GetLights(){
    vec3 n = normalize(Normal);
    vec3 t = normalize(Tangent);
    vec3 b = cross(n,t);
    mat3 tbnMatrix = mat3(t.x, b.x, n.x,
                          t.y, b.y, n.y,
                          t.z, b.z, n.z);
    vec3 vToEye = normalize(VertexToEye);
    vec3 bump = normalize(texture(NormalMap, Texcoord*scale.x).xyz*2-1);//+vec3(0,cos(Texcoord.y+time)/64,0);
    vec4 FINALLIGHT = GlobalAmbient*ObjectAmbient;

    for(int i=0;i<lightCount&&i<lightMax;++i){
        /* VERTEX <-> LIGHT VECTORS */
        vec3 LightToVertex = vec3(LightPositions[i]-VertexPosition.xyz);   //Surface to light
        float Distance = length(LightToVertex);
        LightToVertex = normalize(LightToVertex);

        vec3 LightToSurface = tbnMatrix*LightToVertex;
        vec3 NormalSurfaceToLight = normalize(LightToSurface);
        vec3 R = normalize(reflect(-vToEye,bump));
        float Direction;
        if(LightDirections[i] == vec3(0,0,0))
        {
            Direction = 1;
        }
        else
        {
            Direction = dot(normalize(LightDirections[i]),normalize(-LightToVertex));
        }
        float BumpDiffuse=max(dot(bump,NormalSurfaceToLight),0);
        float Diffuse =max(dot(n,LightToVertex),0);
        float Specular= pow(clamp(dot(NormalSurfaceToLight,R),0,1),max(Shininess,1));
        // COLORIZE LIGHTS
        if(degrees(acos(Direction))<=LightCutoffs[i]){

            // CALCULATE ATTENUATION
            float AttenuationFinal = pow(Direction,LightSpotExponents[i]);
            float AttenuationFactor =(LightConstantAttenuations[i]+LightLinearAttenuations[i]*Distance+LightQuadraticAttenuations[i]*Distance*Distance);
            if(AttenuationFactor!=0){
                AttenuationFinal = AttenuationFinal/AttenuationFactor;
            }
                FINALLIGHT += ObjectAmbient*LightAmbients[i];
                FINALLIGHT += AttenuationFinal*LightDiffuses[i]*ObjectDiffuse*Diffuse*BumpDiffuse*Direction;
            if(Diffuse*Direction>0){
                FINALLIGHT += AttenuationFinal*LightSpeculars[i]*ObjectSpecular*Diffuse*Specular*Direction;
            }
       }
    }
    return vec4(FINALLIGHT.xyz,1);
}

void main() {

    outColor = vec4(1,1,1,1);
    if(LightsOn)
        outColor *= GetLights();
    if(TextureOn)
        outColor *= texture(ObjectTexture,Texcoord);
    if(ColorOn)
        outColor *= color;
    if(ZeroOn)
        outColor *= vec4(0,0,0,1);
}
