#version 130
in vec3 inPos;
in vec2 inUV;
in vec3 inNorm;

uniform mat4 PV;
uniform mat4 M;

out vec4 outPos;
out vec3 outNorm;
out vec2 outUV;

void main(void)
{
    outPos = M * vec4(inPos,1);
    outNorm = (M * vec4(inNorm,1)).xyz;
    outUV = inUV;
    gl_Position = PV * outPos; 
}
