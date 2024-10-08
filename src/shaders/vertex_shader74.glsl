#version 430

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNormal;
layout(location = 2) in vec2 texCoord;  // Cambié el location a 2 para evitar conflicto

out vec4 varyingColor;
out vec3 varyingNormal;
out vec3 varyingLightDir;
out vec3 varyingVertexPos;
out vec2 tc;  // Coordenadas de textura

struct PositionalLight {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position;
};

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

void main(void) {
    varyingVertexPos = (mv_matrix * vec4(vertPos, 1.0)).xyz;
    varyingLightDir = light.position - varyingVertexPos;
    varyingNormal = (norm_matrix * vec4(vertNormal, 1.0)).xyz;
    tc = texCoord;  // Pasar las coordenadas de textura al fragment shader

    gl_Position = proj_matrix * mv_matrix * vec4(vertPos, 1.0);
}
