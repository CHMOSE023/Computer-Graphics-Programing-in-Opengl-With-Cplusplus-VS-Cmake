#version 430

in vec3 vNormal;
in vec3 vVertPos;

out vec4 fragColor;

uniform vec3 eyePos;
layout (binding = 0) uniform samplerCube t;

void main(void)
{
    //float ratio = 1.00 / 1.52;
    vec3 I = normalize(vVertPos - eyePos);
    //vec3 R = refract(I, normalize(vNormal), ratio);
    vec3 R = reflect(I, normalize(vNormal));
    fragColor = vec4(texture(t, R).rgb, 1.0);

}