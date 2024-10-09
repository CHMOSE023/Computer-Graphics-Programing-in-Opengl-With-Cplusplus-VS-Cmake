#version 430
layout (location=0) in vec3 vertPos;
layout (location=1) in vec3 vertNor;

uniform mat4 shadowMVP;

void main(void)
{	gl_Position = shadowMVP * vec4(vertPos,1.0);
}
