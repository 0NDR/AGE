{
#startVertex
#version 150
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require


layout(location = 0)in vec2 position;
layout(location = 2)in vec2 texcoord;

out vec4 Color;
out vec2 Texcoord;

uniform mat4 model,proj;

void main()
{
    Texcoord = texcoord;
    gl_Position = proj*model*vec4(position,0.0f, 1.0 );

}

#endVertex
}
{
#startFragment
	#version 150
	in vec2 Texcoord;
	out vec4 outColor;
	uniform vec4 color;
    uniform sampler2D disptext;
    uniform sampler2D disptext2;
	void main() {
        vec4 accum =   texelFetch(disptext, ivec2(gl_FragCoord.xy), 0);
        float reveal = texelFetch(disptext2, ivec2(gl_FragCoord.xy), 0).r;
        outColor =vec4(accum.rgb / max(accum.a, 1e-5), reveal);
        if(accum.a==reveal)
            outColor=vec4(1,1,1,1);
    }

#endFragment
}

