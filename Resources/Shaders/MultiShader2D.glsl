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
    uniform sampler2D bkg;
    uniform int isbkg;
    float maxComponent(vec4 a)
    {
        float b=a.x,c=a.y;
        if(a.x<a.z)
            b=a.z;
        return b>c?b:c;

    }
	void main() {
        ivec2 C = ivec2(gl_FragCoord.xy);
        if(isbkg==1)
        {
            outColor=texture2D(bkg,Texcoord);
        }
        else
        {
            float  revealage = texelFetch(disptext2, C, 0).r;
            if (revealage <= 0.0) {discard;} //discard if opaque

            vec4 accum     = texelFetch(disptext, C, 0);
            vec3 averageColor = accum.rgb / max(accum.a, 1e-10);


            // dst' =  (accum.rgb / accum.a) * (1 - revealage) + dst * revealage
            //outColor = vec4(averageColor,1-revealage);
            outColor = vec4(accum);
        }
    }

#endFragment
}

/*
	void main() {
        vec4 accum = texture2D(disptext, Texcoord);
        float r = accum.a;
        accum.a = texture2D(disptext2, Texcoord).r;
        if (r >= 1.0) {
            discard;
        }
        outColor = vec4(accum.rgb / clamp(accum.a, 1e-4, 5e4), r);
    }
*/
/*
    float maxComponent(vec4 a)
    {
        float b=a.x,c=a.y;
        if(a.x<a.z)
            b=a.z;
        return b>c?b:c;

    }
	void main() {
        ivec2 C = ivec2(gl_FragCoord.xy);
        float  revealage = texelFetch(disptext2, C, 0).r;
        if (revealage == 1.0) {
            // Save the blending and color texture fetch cost
            discard;
        }

        vec4 accum     = texelFetch(disptext, C, 0);
        // Suppress overflow
        if (isinf(maxComponent(abs(accum)))) {
            accum.rgb = vec3(accum.a);
        }
        vec3 averageColor = accum.rgb / max(accum.a, 0.00001);


        // dst' =  (accum.rgb / accum.a) * (1 - revealage) + dst * revealage
        outColor = vec4(averageColor, 1.0 - revealage);
        */
