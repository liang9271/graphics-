#pragma once
#include "Shape.h"

class Sphere: public Shape {
public: 
Sphere();

Sphere(const point3& cente, double radius);

bool intersect(const ray& r, HitSphere& hit) override;

private:

point3 center;
double radius;
};
