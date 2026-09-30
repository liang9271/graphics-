#include "Sphere.h"

Sphere::Sphere()
: center(0,0,0), radius(1.0)
{
}

Sphere::Sphere(const point3& center, double radius)
:center(center),radius(radius)
{
}

bool Sphere::intersect(const ray& r, HitSphere& hit)
{
vec3 oc = r.origin() - center;

double a = dot(r.direction(), r.direction());
double b = 2.0 * dot(oc, r.direction());
double c = dot(oc, oc) - radius * radius;

double discriminant = b * b - 4.0 * a * c;

if (discriminant <0) {
        return false;
}

double t = (-b - std::sqrt(discriminant)) / (2.0 * a);

hit.t = t;
hit.p = r.at(t);
hit.normal = (hit.p - center) / radius;
hit.r = r; //save the ray that hit the sphere
return true;

}
