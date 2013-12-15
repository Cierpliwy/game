#version 130
in vec3 inPos;
uniform mat4 MVP;

void main(void)
{
    gl_Position = MVP * vec4(inPos, 1);
}
