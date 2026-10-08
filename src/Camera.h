#pragma once

#include "ray.h"

class Camera {
public:
    Camera(
        const point3& position,
        const vec3& viewDirection,
        double focalLength,
        double imagePlaneWidth
    );

    ray getRay(
        double u,
        double v,
        double aspectRatio
    ) const;

private:
    point3 position;
    vec3 forward;
    vec3 right;
    vec3 up;

    double focalLength;
    double imagePlaneWidth;
};