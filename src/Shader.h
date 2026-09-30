#pragma once

#include "Shape.h"

using color = vec3;

struct HitSphere;

class Shader {
public:
//every shader provides its own raycolor()
virtual color rayColor(const HitSphere &h) = 0;
virtual ~Shader() = default;

};
