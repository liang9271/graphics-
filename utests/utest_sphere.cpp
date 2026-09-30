#include <catch2/catch_test_macros.hpp>
#include "Sphere.h"

TEST_CASE("Sphere hit")
{
    Sphere sphere(point3(0, 0, -5), 1.0);

    ray r(point3(0, 0, 0), vec3(0, 0, -1));

    HitSphere hit;

    REQUIRE(sphere.intersect(r, hit) == true);
}

TEST_CASE("Sphere miss")
{
    Sphere sphere(point3(0, 0, -5), 1.0);

    ray r(point3(0, 0, 0), vec3(0, 2, -1));

    HitSphere hit;

    REQUIRE(sphere.intersect(r, hit) == false);
}