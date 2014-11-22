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

vec4 bloom(sampler2D frame, vec2 texcoord)
{
   vec4 sum = vec4(0);
   int j;
   int i;
   vec4 retVal;
   for( i= -4 ;i < 4; i++)
   {
        for (j = -3; j < 3; j++)
        {
            sum += texture2D(frame, texcoord + vec2(j, i)*0.004) * 0.25;
        }
   }
    if (texture2D(frame, texcoord).r < 0.3)
    {
       retVal = sum*sum*0.012 + texture2D(frame, texcoord);
    }
    else
    {
        if (texture2D(frame, texcoord).r < 0.5)
        {
            retVal = sum*sum*0.009 + texture2D(frame, texcoord);
        }
        else
        {
            retVal = sum*sum*0.0075 + texture2D(frame, texcoord);
        }
    }
    return retVal;
}

void main()
{
     vec4 colorNoRays = texture2D(FrameBuffer,Texcoord);
     outColor = bloom(FrameBuffer, Texcoord);

}
