#version 130
in vec4 pos;
in vec2 uv;

uniform vec2 light;
uniform float lightSize;
uniform sampler2D texSampler;

out vec3 outColor;

void main(void) {
    float d = distance(pos.xy, light) / lightSize;
    float z = clamp(1-d,0,1);
    outColor = texture(texSampler, uv).rgb*z; 
}

