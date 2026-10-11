#include "MirrorShader.h"
#include "Scene.h"
#include "SpaceBackground.h"
#include "MirrorShader.h"

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
    vec3 incoming = unit_vector(h.r.direction());
    vec3 reflectedDirection =
        incoming - 2.0 * dot(incoming, h.normal) * h.normal;

    reflectedDirection =
        unit_vector(reflectedDirection);
    point3 reflectionOrigin =
        h.p + 0.0001 * h.normal;
    ray reflectionRay(
        reflectionOrigin,
        reflectedDirection
    );
    HitSphere reflectionHit;
    std::shared_ptr<Shape> reflectionShape;    
    #include "Scene.h"
    #include "SpaceBackground.h"
    
    // Constructor for the mirror shader.
    MirrorShader::MirrorShader()
    {
    }
    
    // This function decides the color for a hit point.
    // h = information about the ray hit
    // scene = all objects in the scene
    // depth = how many more bounces this ray is allowed to do
    color MirrorShader::rayColor(
        const HitSphere& h,
        const Scene& scene,
        int depth)
    {
        // Stop reflection recursion if it's too deep.
        // This prevents infinite mirror reflections.
        if (depth <= 0) {
            return color(0, 0, 0);  // black
        }
    
        // incoming = direction the ray is traveling
        // unit_vector makes it a length-1 direction vector
        vec3 incoming = unit_vector(h.r.direction());
    
        // Reflect the incoming direction across the surface normal.
        // Formula: r = i - 2*(i·n)*n
        vec3 reflectedDirection =
            incoming - 2.0 * dot(incoming, h.normal) * h.normal;
    
        // Normalize the reflected direction again to keep it a unit vector.
        reflectedDirection = unit_vector(reflectedDirection);
    
        // Start the reflected ray just a tiny bit above the surface
        // so it does not immediately hit the same surface again.
        point3 reflectionOrigin =
            h.p + 0.0001 * h.normal;
    
        // Create the new reflected ray.
        ray reflectionRay(
            reflectionOrigin,
            reflectedDirection
        );
    
        // Variables to store the hit result of the reflected ray.
        HitSphere reflectionHit;
        std::shared_ptr<Shape> reflectionShape;
    
        // Check if the reflected ray hits something in the scene.
        if (scene.intersect(
                reflectionRay,
                reflectionHit,
                reflectionShape)) {
            // If it hits something, compute that object's color.
            // Decrease depth so reflections do not continue forever.
            return reflectionShape->getShader()->rayColor(
                reflectionHit,
                scene,
                depth - 1
            );
        }
    
        // If the reflected ray misses everything,
        // return the background color in that reflected direction.
        return spaceBackground(reflectedDirection);
    }

    if (scene.intersect(
            reflectionRay,
            reflectionHit,
            reflectionShape)) {
        return reflectionShape->getShader()->rayColor(
            reflectionHit,
            scene,
            depth - 1
        );
    }
    return spaceBackground(reflectedDirection);
}
