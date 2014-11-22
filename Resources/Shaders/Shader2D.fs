#version 150
#define pi 3.14159265659

const int LightNumber = 1;

out vec4 outColor;

in vec4 VertexPosition;
in vec2 Texcoord;

uniform vec4 ObjectColor;
uniform vec2 resolution;
uniform sampler2D ObjectTexture;
uniform float time;
uniform int xory;
uniform float waterlevel;
uniform float depthAMT;
uniform sampler2D FrameBuffer;
uniform sampler2D DepthBuffer;
float blurSize = 1.0/512.0;
float getDistance(vec2 a, vec2 b)
{
    return sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2));
}
vec4 squareShade(vec2 position, int layers, int start)
{
    vec4 final = vec4(1,1,1,1);
    for(float i=start;i<start+layers;i++)
    {
            float amt = pow(2,i);
            float posx = cos(position.x*2*pi*(amt) + pi/2);
            float posy = sin(position.y*2*pi*(amt));
            if(((posx<0&&posy<0) || (posx>0&&posy>0))&&final==vec4(1,1,1,1))
            {
                float mult = 1/(i+1-start);
                final -= vec4(mult,mult,mult,0);
            }
   }
    return final;
}
vec4 blur(bool isHorizontal, float sigma, vec2 vertTexcoord, sampler2D img,vec2 Resolution)
{
  float numBlurPixelsPerSide = 3.0;//float(blurSize / 2);

  vec2 blurMultiplyVec = isHorizontal ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  vec2 blurAdd = (blurMultiplyVec*Resolution);
  float blurSize = 1.0/(blurAdd.x+blurAdd.y);

  // Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
  vec3 incrementalGaussian;
  incrementalGaussian.x = 1.0 / (sqrt(2.0 * pi) * sigma);
  incrementalGaussian.y = exp(-0.5 / (sigma * sigma));
  incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;

  vec4 avgValue = vec4(0.0, 0.0, 0.0, 0.0);
  float coefficientSum = 0.0;

  // Take the central sample first...
  avgValue += texture2D(img, vertTexcoord.st) * incrementalGaussian.x;
  coefficientSum += incrementalGaussian.x;
  incrementalGaussian.xy *= incrementalGaussian.yz;

  // Go through the remaining 8 vertical samples (4 on each side of the center)
  for (float i = 1.0; i <= numBlurPixelsPerSide; i++) {
    avgValue += texture2D(img, vertTexcoord.st - i * blurSize*blurMultiplyVec) * incrementalGaussian.x;
    avgValue += texture2D(img, vertTexcoord.st + i * blurSize*blurMultiplyVec) * incrementalGaussian.x;
    coefficientSum += 2.0 * incrementalGaussian.x;
    incrementalGaussian.xy *= incrementalGaussian.yz;
  }
  return avgValue/coefficientSum;
}

float LinearizeDepth(vec2 uv)
{
  float n = .01; // camera z near
  float f = 200.0; // camera z far
  float z = texture2D(DepthBuffer, uv).r;
  return (2.0 * n) / (f + n - z * (f - n));
}
float getBlur(float depth, float amt, float amtint)
{
    float sub = depth-amt;
    if(sub>0)//&&sub<amtint)
    {
        float numer = 3.9/amtint;
        return (.1)+numer*sub;
    }/*
    else if(sub>amtint)
    {
        return 8;
    }*/
    else if(sub<0)
    {
        return 0.1;
    }
}

void main() {

    vec4 Texture = texture(FrameBuffer,Texcoord);
    /*outColor = color;
    if(xory ==1)
    {
        if(Texture.x<=0.05)
            outColor = vec4(0,0,1,1);
        else if(Texture.x>0.05 && Texture.x<=.1)
            outColor = vec4(.5,.5,1,1);
        else if(Texture.x>0.1 && Texture.x<=.2)
            outColor = vec4(1,1,0,1);
        else if(Texture.x>.2&&Texture.x<=.50)
            outColor = vec4(0,1,0,1);
        else if(Texture.x>.5&&Texture.x<=.68)
            outColor = vec4(.65,.16,.16,1);
        else if(Texture.x>.68&&Texture.x<=.85)
            outColor = vec4(.5,.5,.5,1);
        else if(Texture.x>.85)
            outColor = vec4(1,1,1,1);
    }*/
    outColor=ObjectColor*Texture;
}






