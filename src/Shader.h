#pragma once

#include "Shape.h"

using color = vec3;

struct HitSphere;

class Scene;

class Shader {
public:
    virtual color rayColor(
        const HitSphere& h,
        const Scene& scene,
        int depth
    ) = 0;

    virtual ~Shader() = default;
};