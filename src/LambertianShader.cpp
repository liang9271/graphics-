#include "LambertianShader.h"
#include "Scene.h"
#include <algorithm>

LambertianShader::LambertianShader(const color& diffuseColor)
    : diffuseColor(diffuseColor)
{
}

color LambertianShader::rayColor(
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

    color result(0, 0, 0);
    for (const auto& light : lights) {
        const vec3 toLight = light.position - h.p;
        const double lightDistance = toLight.length();
        if (lightDistance <= 0.0) {
            continue;
        }

        const vec3 lightDirection = toLight / lightDistance;
        const ray shadowRay(h.p + 0.0001 * h.normal, lightDirection);
        HitSphere shadowHit;
        std::shared_ptr<Shape> shadowShape;
        if (scene.intersect(shadowRay, shadowHit, shadowShape) &&
            shadowHit.t < lightDistance) {
            result += diffuseColor * light.intensity * 0.2;
            continue;
        }

        const double nDotL =
            std::max(0.0, dot(h.normal, lightDirection));
        result += diffuseColor * light.intensity * nDotL;
    }
    return result;
}