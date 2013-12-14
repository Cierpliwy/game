#version 130
in vec3 inPos;
in vec2 inUV;

uniform mat4 MVP;

out vec4 pos;
out vec2 uv;

void main(void)
{
    pos = vec4(inPos, 1);
    gl_Position = MVP * pos;
    uv = inUV;
}
