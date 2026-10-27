#pragma once

#include "color.h"

inline color spaceBackground(const vec3& direction) {
    const vec3 unitDirection = unit_vector(direction);
    const double vertical = 0.5 * (unitDirection.y() + 1.0);
    const color top(0.53, 0.81, 0.98);
    const color bottom(0.68, 0.85, 1.0);
    return (1.0 - vertical) * bottom + vertical * top;
}
