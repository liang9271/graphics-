#include "BlinnPhongShader.h"
#include "Scene.h"

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

color BlinnPhongShader::rayColor(
    const HitSphere& h,
    const Scene& scene,
    int depth)
{
    const std::vector<Scene::PointLight> fallbackLights = {
        {point3(0, 10, 5), color(1, 1, 1)}
    };
    const auto& lights = scene.getPointLights().empty()
        ? fallbackLights
        : scene.getPointLights();
    const vec3 viewDirection = unit_vector(-h.r.direction());

    color result(0, 0, 0);
    for (const auto& light : lights) {
        const vec3 toLight = light.position - h.p;
        if (toLight.length_squared() <= 0.0) {
            continue;
        }
        const vec3 lightDirection = unit_vector(toLight);
        const double nDotL =
            std::max(0.0, dot(h.normal, lightDirection));
        const vec3 halfVector = unit_vector(lightDirection + viewDirection);
        const double nDotH =
            std::max(0.0, dot(h.normal, halfVector));
        const double specular = std::pow(nDotH, phongExponent);

        result += light.intensity *
            (diffuseColor * nDotL + specularColor * specular);
    }

    result[0] = std::clamp(result[0], 0.0, 1.0);
    result[1] = std::clamp(result[1], 0.0, 1.0);
    result[2] = std::clamp(result[2], 0.0, 1.0);
    return result;
}
