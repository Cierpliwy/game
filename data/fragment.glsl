#version 130
in vec4 pos;
in vec2 uv;

uniform vec2 light;
uniform float lightSize;
uniform sampler2D texSampler;

out vec4 outColor;

void main(void) {
    float d = distance(pos.xy, light) / lightSize;
    float z = clamp(1-d,0,1);
    vec4 color = texture(texSampler, uv);
    outColor = vec4(color.rgb*z,color.a); 
}

