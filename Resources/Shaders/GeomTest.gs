#version 150
#define pi 3.14159265759
layout(triangles) in;
layout (triangle_strip, max_vertices=9) out;

in vec2 Texcoord[3];
in vec3 Normal[3],Tangent[3];
in vec3 Texcoord3d[3];
out vec3 normalo;
out vec2 texcoordo;
out vec3 tangento;
out vec3 texcoord30;
uniform mat4 model, view, proj;
uniform int time;
uniform vec3 viewpos;
uniform sampler2D Texture_Normal0;

float parallaxMapping(vec3 V,sampler2D Height,vec2 texcoord,vec2 scale)
{
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

   return heightFromTexture;
}



void main()
{
  for(int i = 0; i < gl_in.length(); i++)
  {
     // copy attributes
    vec3 n = normalize(Normal[i]);                                     //unit Normal vector
    vec3 t = normalize(Tangent[i]);                                    //unit Tangent vector
    vec3 b = cross(n,t);
    vec3 V = normalize(mat3(t,b,n)*normalize((model*vec4(Texcoord3d[i],1)).xyz-viewpos));
    float height = parallaxMapping(V,Texture_Normal0,Texcoord[i],vec2(.01,0));

    gl_Position = proj*view*model*vec4(Texcoord3d[i]+vec3(0,height*-.1,0),1);
    normalo = Normal[i];
    texcoordo = Texcoord[i];
    tangento = Tangent[i];
    texcoord30 = Texcoord3d[i];
    // done with the vertex
    EmitVertex();

  }
  EndPrimitive();
}
