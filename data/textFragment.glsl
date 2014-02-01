#version 130
in vec2 outUV;
in vec4 outPos;

uniform sampler2D tex;
uniform float char;
uniform float time;
uniform bool black;

out vec4 outColor;

void main(void) {
    float charSize = 1.0/16.0;
    vec2 charUV = vec2(charSize*mod(char, 16) + charSize*outUV.x,
                       charSize*floor(char/16.0) + charSize*outUV.y);
    vec4 color = texture(tex, charUV);

    if (color.a < 0.1) discard;

    if (black) {
        outColor = vec4(0,0,0,color.a);
        return;
    }

    float off = outPos.x*5 + time*2;
    
    color.r = (sin(off*1.5)+1)/4+0.5;
    color.g = mod(off,1)/4+0.75;
    color.b = mod(off*1.5,1)/2+0.5;

    outColor = color;
}

