#version 130
#define M_PI 3.1415926535897932384626433832795
in vec3 outPos;
in vec2 outUV;
in vec3 outNorm;

uniform sampler2D tex;
uniform bool white;
uniform vec2 playerPos;
uniform sampler1D shadow;

out vec4 outColor;

void main(void) {
	if (white) {
        outColor = vec4(1,1,1,1);
        return;
    }
    
    outColor = texture(tex, outUV);

    float dist = distance(outPos.xy, playerPos);
    float dx = outPos.x-playerPos.x;
    float dy = outPos.y-playerPos.y;

    float angle = atan(dx/dy);
    if (dy < 0) angle += M_PI;
    else if (dx < 0) angle += 2*M_PI;
    angle /= 2*M_PI;

    float coll = texture(shadow, angle).r;
    if (dist > coll) outColor.rgb *= 1-clamp((dist-coll)/4,0,1)+0.1;
}

