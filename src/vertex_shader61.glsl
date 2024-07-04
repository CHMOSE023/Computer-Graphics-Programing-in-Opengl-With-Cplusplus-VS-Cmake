#version 410
layout(location = 0) in vec3 vPosition;
uniform mat4 proj_matrix;
uniform mat4 mv_matrix;
void main(void) {
    gl_Position = proj_matrix * mv_matrix * vec4(vPosition, 1.0);
}
