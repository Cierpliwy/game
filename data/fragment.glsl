#version 130
out vec4 outColor;
in vec4 pos;
uniform vec2 light;
uniform float lightSize;

void main(void) {
    float d = distance(pos.xy, light) / lightSize;
    float z = clamp(1-d,0,1);
    outColor = vec4(z,0,0,1);
}

