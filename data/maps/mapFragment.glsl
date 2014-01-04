#version 130
#define M_PI 3.1415926535897932384626433832795
in vec4 pos;
in vec2 uv;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler1D shadow;
uniform float vis;
uniform bool grid;
uniform vec2 playerPos;

out vec4 outColor;

void main(void) {
    if (grid) {
        outColor = vec4(1,1,1,1);
        return;
    }
    float v = clamp(vis, 0, 1);
    if (v < texture(tex1, uv).r) discard;

    vec4 color = texture(tex0, uv);
    float dist = distance(pos.xy, playerPos);
    float dx = pos.x-playerPos.x;
    float dy = pos.y-playerPos.y;

    float angle = atan(dx/dy);
    if (dy < 0) angle += M_PI;
    else if (dx < 0) angle += 2*M_PI;
    angle /= 2*M_PI;

    float coll = texture(shadow, angle).r;
    if (dist > coll) color.rgb *= 1-clamp((dist-coll)/5,0,1)+0.1;
    
    outColor = color; 
}

