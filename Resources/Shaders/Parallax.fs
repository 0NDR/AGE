#version 150
#define pi 3.14159265659
out vec4 outColor;

in vec4 VertexPosition,color;
in vec2 texcoordo;
in vec3 normalo,tangento,texcoord30;

vec2 SteepParallaxMapping(mat3 tbnMatrix,sampler2D Height,vec2 texcoord,vec2 scale)
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
vec2 parallaxOcclusionMapping(mat3 tbnMatrix,sampler2D Height,vec2 texcoord,vec2 scale)
{
    vec3 V = normalize(tbnMatrix*(VertexPosition.xyz-viewpos));
   // determine optimal number of layers
   const float minLayers = 10;
   const float maxLayers = 15;
   float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0, 0, 1), V)));

   // height of each layer
   float layerHeight = 1.0 / numLayers;
   // current depth of the layer
   float curLayerHeight = 0;
   // shift of texture coordinates for each layer
   vec2 dtex = scale.x * V.xy / V.z / numLayers;

   // current texture coordinates
   vec2 currentTextureCoords = texcoord;

   // depth from heightmap
   float heightFromTexture = 1-texture(Height, currentTextureCoords).a;

   // while point is above the surface
   while(heightFromTexture > curLayerHeight && curLayerHeight<=1)
   {
      // to the next layer
      curLayerHeight += layerHeight;
      // shift of texture coordinates
      currentTextureCoords -= dtex;
      // new depth from heightmap
      heightFromTexture = 1-texture(Height, currentTextureCoords).a;
   }

   ///////////////////////////////////////////////////////////

   // previous texture coordinates
   vec2 prevTCoords = currentTextureCoords + dtex;

   // heights for linear interpolation
   float nextH	= heightFromTexture - curLayerHeight;
   float prevH	= 1-texture(Height, prevTCoords).a
                           - curLayerHeight + layerHeight;

   // proportions for linear interpolation
   float weight = nextH / (nextH - prevH);

   // interpolation of texture coordinates
   vec2 finalTexCoords = prevTCoords * weight + currentTextureCoords * (1.0-weight);

   // interpolation of depth values
   //parallaxHeight = curLayerHeight + prevH * weight + nextH * (1.0 - weight);

   // return result
   return finalTexCoords;
}
vec2 simpleParallax(mat3 tbnMatrix,sampler2D Height,vec2 texcoord,vec2 scale)
{
    vec3 v = normalize(tbnMatrix*(viewpos-VertexPosition.xyz));
    float height = 1-texture2D(Height, texcoord).a;
    float hsb = height*scale.x+scale.y;
    return texcoord+(v.xy*hsb);
}

void main()
{
}
