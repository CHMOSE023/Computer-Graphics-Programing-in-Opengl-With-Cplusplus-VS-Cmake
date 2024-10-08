#version 430

in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertexPos;
in vec2 tc;  // Coordenadas de textura

out vec4 fragColor;

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
uniform sampler2D textureSampler;  // Uniform para la textura

void main(void) {
    vec3 L = normalize(varyingLightDir);
    vec3 N = normalize(varyingNormal);
    vec3 V = normalize(-varyingVertexPos);

    vec3 R = normalize(reflect(-L, N));

    float cosTheta = dot(N, L);
    float cosPhi = dot(R, V);

    vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
    vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta, 0.0);

    vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi, 0.0), material.shininess);

    vec4 textureColor = texture(textureSampler, tc);  // Obtener el color de la textura

    vec3 finalColor = (ambient + diffuse + specular) * textureColor.rgb;

    fragColor = vec4(finalColor, 1.0);
}