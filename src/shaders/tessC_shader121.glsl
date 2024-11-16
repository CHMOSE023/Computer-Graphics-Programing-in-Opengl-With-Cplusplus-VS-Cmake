#version 430

uniform mat4 mvp_matrix;
layout (vertices = 1) out;

void main(void)
{	gl_TessLevelOuter[0] = 3;
	gl_TessLevelOuter[2] = 3;
	gl_TessLevelOuter[1] = 3;
	gl_TessLevelOuter[3] = 3;
	gl_TessLevelInner[0] = 6;
	gl_TessLevelInner[1] = 3;
}