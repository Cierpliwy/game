#version 130
in vec4 pos;
in vec2 uv;

uniform vec2 light;
uniform float lightSize;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float vis;
uniform bool grid;

out vec4 outColor;

void main(void) {
    if (grid) {
        outColor = vec4(0,1,1,1);
        return;
    }
    float v = clamp(vis, 0, 1);
    if (v < texture(tex1, uv).r) discard;

    // calculate light
    float dist = clamp(distance(pos.xy, light)/lightSize, 0, 1);
    vec4 color = texture(tex0, uv);
    float flash = clamp((1/dist-2),0,2)*texture(tex1, uv).g;
    color = vec4(color.rgb+vec3(1,0.5,0.2)*flash, color.a);
    color = vec4(color.rgb*(1-dist), color.a);

    outColor = color; 
}

