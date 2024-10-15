#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 vNormal;
out vec3 vVertPos;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
layout (binding = 0) uniform samplerCube t;

void main(void)
{
	vNormal = mat3(transpose(inverse(mv_matrix))) * normal;
	vVertPos = vec3(mv_matrix * vec4(position,1.0));
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
}
