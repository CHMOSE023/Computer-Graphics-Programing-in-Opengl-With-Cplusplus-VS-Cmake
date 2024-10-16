#version 430

in vec3 vNormal;
in vec3 vVertPos;
out vec4 fragColor;

uniform vec3 eyePos;
layout (binding = 0) uniform samplerCube t;

void main(void)
{
    
    float ratio = 1.00 / 2.42; 
    vec3 viewDir = normalize(eyePos - vVertPos);
    vec3 r = -refract(viewDir, normalize(vNormal), ratio);
    fragColor = vec4(texture(t, r).rgb, 1.0);
    
}