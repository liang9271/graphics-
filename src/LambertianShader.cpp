#include "LambertianShader.h"

#include <algorithm>

LambertianShader::LambertianShader(const color& diffuseColor)
    : diffuseColor(diffuseColor)
{
}

color LambertianShader::rayColor(const HitSphere& h)
{
    // point light
    vec3 lightPosition(0, 10, 5);
    vec3 lightDir = unit_vector(lightPosition - h.p);

    double nDotL = std::max(0.0, dot(h.normal, lightDir));
    return diffuseColor * nDotL;
}
