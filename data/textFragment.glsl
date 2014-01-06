#version 130
in vec2 outUV;

uniform sampler2D tex;
uniform float char;

out vec4 outColor;

void main(void) {
    float charSize = 1.0/16.0;
    vec2 uv = clamp(outUV, vec2(0.1,0.1), vec2(0.9,0.9));
    vec2 charUV = vec2(charSize*mod(char, 16) + charSize*uv.x,
                       charSize*floor(char/16.0) + charSize*uv.y);
    outColor = texture(tex, charUV);
}

