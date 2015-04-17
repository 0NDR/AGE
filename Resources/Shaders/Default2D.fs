	#version 150
	in vec2 Texcoord;
	out vec4 outColor;
	uniform vec4 color;
    uniform sampler2D disptext;
	void main() {
		outColor = texture(disptext,Texcoord)*color;
	}
