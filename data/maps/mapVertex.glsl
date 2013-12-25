#version 130
in vec3 inPos;      // input vertices
in vec2 inUV;       // texture coordinates

uniform mat4 PV;    // projection * view matrix
uniform float vis;  // visibility 

out vec4 pos;       // position in global coordinates
out vec2 uv;        // interpolated uv coordinates

void main(void)
{
    pos = vec4(inPos, 1);
    float v = clamp(vis,0,1);
    pos.x = pos.x + sin(pos.x*0.1 + v*20)*2*(1-v);
    pos.y = pos.y + sin(pos.y*0.1 + v*20)*2*(1-v);
    pos.z = pos.z + (1-v);
    vec4 PVM = PV * pos;
    gl_Position = PVM;
    uv = inUV;
}
