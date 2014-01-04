#version 130
in vec3 inPos;
in vec2 inUV;

out vec4 outPos;
out vec2 outUV;

void main(void)
{
    outPos = vec4(inPos,1);
    outUV = inUV;
    gl_Position = outPos; 
}
