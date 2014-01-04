#version 130
#define M_PI 3.1415926535897932384626433832795
in vec4 outPos;
in vec2 outUV;

uniform sampler2D tex;
uniform float res;
uniform vec2 pos;
uniform vec2 size;

out vec4 outColor;

void main(void) {
    float maxSize = max(size.x, size.y);
    float dist = maxSize;
    float angle = M_PI*2*outUV.x;
    
    for(float y = 0.0f; y <= maxSize*2; y += res) {
        vec2 newPos = pos + vec2(y*sin(angle), y*cos(angle)); 
        newPos /= size;
        float val = texture(tex, newPos).r;
        if (val == 1) {
            dist = y;
            break;
        }
    }
    outColor = vec4(vec3(dist),1);
}

