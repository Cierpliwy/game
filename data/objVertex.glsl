#version 130
in vec3 in1Pos;
in vec2 in2UV;
in vec3 in3Norm;

uniform mat4 PV;
uniform mat4 M;

out vec3 outPos;
out vec2 outUV;
out vec3 outNorm;

void main(void)
{
    outPos = (M * vec4(in1Pos,1)).xyz;
	outUV = in2UV;
    outNorm = (M * vec4(in3Norm,1)).xyz;
    gl_Position = PV * vec4(outPos,1); 
}
