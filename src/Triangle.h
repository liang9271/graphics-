#pragma once

#include "Shape.h"

class Triangle : public Shape {
public:
    Triangle();
    Triangle(const point3& v0, const point3& v1, const point3& v2);

    bool intersect(const ray& r, HitSphere& hit) override;

private:
    point3 v0;
    point3 v1;
    point3 v2;
};
