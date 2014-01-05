#version 130
in vec3 in1Pos;
in vec2 in2UV;

uniform mat4 PVM;

out vec2 outUV;

void main(void)
{
	outUV = in2UV;
    gl_Position = PVM * vec4(in1Pos,1); 
}
