#include "BlinnPhongShader.h"

#include <algorithm>
#include <cmath>

BlinnPhongShader::BlinnPhongShader(
    const color& diffuseColor,
    const color& specularColor,
    double phongExponent)
    : diffuseColor(diffuseColor),
      specularColor(specularColor),
      phongExponent(phongExponent)
{
}

color BlinnPhongShader::rayColor(const HitSphere& h)
{
    // Point light
    vec3 lightPosition(0, 10, 5);

    // Direction from the hit point to the light
    vec3 lightDir = unit_vector(lightPosition - h.p);

    // Diffuse lighting
    double nDotL = std::max(0.0, dot(h.normal, lightDir));

    // Direction toward the camera
    vec3 viewDir = unit_vector(-h.r.direction());

    // Blinn-Phong half-vector
    vec3 halfVector = unit_vector(lightDir + viewDir);

    // Specular lighting
    double nDotH = std::max(0.0, dot(h.normal, halfVector));

    double specular = std::pow(nDotH, phongExponent);

    // Combine diffuse + specular
  color diffuse = diffuseColor * nDotL;
color specularHighlight = specularColor * specular;

color result = diffuse + specularHighlight;

// Keep RGB values inside the valid [0, 1] range
result[0] = std::min(1.0, result[0]);
result[1] = std::min(1.0, result[1]);
result[2] = std::min(1.0, result[2]);

return result;
}
