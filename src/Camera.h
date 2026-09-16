#pragma once

#include "ray.h"

class Camera {
public:
    virtual ray generateRay(int i, int j) = 0;
};
