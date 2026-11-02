#include "Triangle.h"
#include <algorithm>
#include <cmath>

Triangle::Triangle()
    : v0(0, 0, -5),
      v1(1, 0, -5),
      v2(0, 1, -5)
{
}

Triangle::Triangle(const point3& v0, const point3& v1, const point3& v2)
    : v0(v0),
      v1(v1),
      v2(v2)
{
}

bool Triangle::intersect(const ray& r, HitSphere& hit)
{
    const double epsilon = 1e-8;

    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;

    // Find the normal of the triangle.
    vec3 normal = unit_vector(cross(edge1, edge2));

    // Check whether the ray is parallel to the triangle.
    double denominator = dot(normal, r.direction());

    if (std::fabs(denominator) < epsilon) {
        return false;
    }

    // Find where the ray intersects the triangle's plane.
    double t = dot(v0 - r.origin(), normal) / denominator;

    if (t < 0) {
        return false;
    }

    point3 p = r.at(t);

    // Check whether p is inside the triangle.
    vec3 c0 = cross(v1 - v0, p - v0);
    vec3 c1 = cross(v2 - v1, p - v1);
    vec3 c2 = cross(v0 - v2, p - v2);

    if (dot(normal, c0) < 0 ||
        dot(normal, c1) < 0 ||
        dot(normal, c2) < 0) {
        return false;
    }

    hit.t = t;
    hit.p = p;
    hit.normal = normal;
    hit.r = r;

    return true;
}

BoundingBox Triangle::boundingBox() const
{
    constexpr double padding = 1e-8;
    point3 minimum(
        std::min({v0.x(), v1.x(), v2.x()}),
        std::min({v0.y(), v1.y(), v2.y()}),
        std::min({v0.z(), v1.z(), v2.z()}));
    point3 maximum(
        std::max({v0.x(), v1.x(), v2.x()}),
        std::max({v0.y(), v1.y(), v2.y()}),
        std::max({v0.z(), v1.z(), v2.z()}));
    for (int axis = 0; axis < 3; ++axis) {
        minimum[axis] -= padding;
        maximum[axis] += padding;
    }
    return BoundingBox(minimum, maximum);
}
