#version 130
in vec3 inPos;
uniform mat4 MVP;
out vec4 pos;

void main(void)
{
    pos = vec4(inPos, 1);
    gl_Position = MVP * pos;
}
