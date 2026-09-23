#pragma
#include "Shape.h"

class Sphere: public Shape {
public: 
Sphere();

Sphere(const point3& cente, double radius);

bool intersect(const ray& r)override;

private:
point3 center;
double radius;
};
