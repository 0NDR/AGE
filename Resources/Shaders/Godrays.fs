#version 150
#define pi 3.14159265659
out vec4 outColor;

in vec4 VertexPosition;
in vec2 Texcoord;
in vec2 lightPositionOnScreen;
uniform sampler2D FrameBuffer;
uniform sampler2D DepthBuffer;

float exposure = .0034f;
float decay = 1;
float density = .84f;
float weight = 5.65f;
const int NUM_SAMPLES = 100 ;

float LinearizeDepth(vec2 uv)
{
  float n = .01; // camera z near
  float f = 200.0; // camera z far
  float z = texture2D(DepthBuffer, uv).r;
  return (2.0 * n) / (f + n - z * (f - n));
}

vec4 godRays(vec2 texcoord)
{
    vec4 returnColor;
    vec2 deltaTextCoord = vec2( texcoord - lightPositionOnScreen.xy );
    deltaTextCoord *= 1.0 /  float(NUM_SAMPLES) * density;
    float illuminationDecay = 1.0;

    for(int i=0; i < NUM_SAMPLES ; i++)
    {
             texcoord -= deltaTextCoord;
             vec4 sample = texture2D(FrameBuffer, texcoord );
             sample *= illuminationDecay * weight;

             returnColor += sample;

             illuminationDecay *= decay;
     }
     returnColor*=exposure;
    return returnColor;
}

void main()
{
     vec4 colorNoRays = texture2D(FrameBuffer,Texcoord);
     vec4 RaysColor = godRays(Texcoord);
     if(RaysColor == vec4(0,0,0,1))
        RaysColor = colorNoRays;
     outColor = RaysColor;

}
