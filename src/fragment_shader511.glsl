#version 430
in vec2 tc;
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout(binding=0) uniform sampler2D samp;

vec4 color1 = texture(samp, tc);
vec4 color2 = vec4(0.5, 0.5, 0.5, 1.0);
float interpolationFactor = 0.1; // Cambia este valor para ajustar la interpolación


void main(void){
color = mix(color1, color2, interpolationFactor);}