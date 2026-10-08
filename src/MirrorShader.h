#pragma once

#include "Shader.h"

class MirrorShader : public Shader {
public:
    MirrorShader();

    color rayColor(
        const HitSphere& h,
        const Scene& scene,
        int depth
    ) override;
};
