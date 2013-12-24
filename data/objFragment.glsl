#version 130
in vec4 outPos;
in vec2 outUV;
in vec3 outNorm;

uniform sampler2D tex;

out vec4 outColor;

void main(void) {
    outColor = texture(tex, outUV);
}

