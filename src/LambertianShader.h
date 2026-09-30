#pragma once

#include "Shader.h"

class LambertianShader : public Shader {

public:

    LambertianShader(const color& diffuseColor);

    color rayColor(const HitSphere& h) override;

private:

    color diffuseColor;
};