#include "MirrorShader.h"

#include "Scene.h"
#include "SpaceBackground.h"

MirrorShader::MirrorShader()
{
}

color MirrorShader::rayColor(
    const HitSphere& h,
    const Scene& scene,
    int depth)
{
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    const vec3 incoming = unit_vector(h.r.direction());
    const vec3 reflectedDirection = unit_vector(
        incoming - 2.0 * dot(incoming, h.normal) * h.normal);
    const point3 reflectionOrigin = h.p + 0.0001 * h.normal;
    const ray reflectionRay(reflectionOrigin, reflectedDirection);

    HitSphere reflectionHit;
    std::shared_ptr<Shape> reflectionShape;
    if (scene.intersect(
            reflectionRay,
            reflectionHit,
            reflectionShape)) {
        return reflectionShape->getShader()->rayColor(
            reflectionHit,
            scene,
            depth - 1);
    }

    return scene.getBackgroundColor().value_or(
        spaceBackground(reflectedDirection));
}
