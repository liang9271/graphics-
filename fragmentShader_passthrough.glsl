#version 330 core

layout(location = 0) out vec4 fragmentColor;

uniform vec3 diffuseComponent;
uniform vec3 specularComponent;
uniform float shininess;
uniform float lightIntensity;

in vec4 normal;
in vec4 lightDir;
in vec4 viewDir;

void main(void)
{
    vec3 N = normalize(normal.xyz);
    vec3 L = normalize(lightDir.xyz);
    vec3 V = normalize(viewDir.xyz);
    vec3 H = normalize(L + V);

    float diffuseFactor = max(dot(N, L), 0.0);
    float specularFactor = 0.0;
    if (diffuseFactor > 0.0) {
        specularFactor = pow(max(dot(N, H), 0.0), shininess);
    }

    vec3 color = diffuseComponent * diffuseFactor
               + specularComponent * specularFactor;
    fragmentColor = vec4(color * lightIntensity, 1.0);
}
