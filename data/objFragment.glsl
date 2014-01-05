#version 130
in vec3 outPos;
in vec2 outUV;
in vec3 outNorm;

uniform sampler2D tex;
uniform bool white;

out vec4 outColor;

void main(void) {
	if (white) outColor = vec4(1,1,1,1);
    else outColor = texture(tex, outUV);
}

