#version 130
in vec2 outUV;

uniform sampler2D tex;
uniform float char;

out vec4 outColor;

void main(void) {
    float charSize = 1.0/16.0;
    vec2 charUV = vec2(charSize*mod(char, 16) + charSize*outUV.x,
                       charSize*floor(char/16.0) + charSize*outUV.y);
    outColor = texture(tex, charUV);
}

