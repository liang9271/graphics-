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
    // Position of the point light.
    vec3 lightPosition(0, 10, 5);

    // Direction from the surface toward the light.
    vec3 toLight = lightPosition - h.p;

    double lightDistance = std::sqrt(dot(toLight, toLight));

    vec3 lightDir =
        unit_vector(toLight);

    // Create a ray from the surface toward the light.
    // Move it slightly away from the surface to avoid
    // accidentally hitting the same surface.
    point3 shadowOrigin =
        h.p + 0.0001 * h.normal;

    ray shadowRay(
        shadowOrigin,
        lightDir
    );

    // Check whether another object blocks the light.
    HitSphere shadowHit;
    std::shared_ptr<Shape> shadowShape;

    if (scene.intersect(
            shadowRay,
            shadowHit,
            shadowShape)) {

        // If the object is between the surface and
        // the light, the point is in shadow.
        if (shadowHit.t < lightDistance) {
            return diffuseColor * 0.2;
        }
    }

    // No object blocks the light, so calculate
    // normal Lambertian lighting.
    double nDotL =
        std::max(0.0, dot(h.normal, lightDir));

    return diffuseColor * nDotL;
}