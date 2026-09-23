#include <catch2/catch_test_macros.hpp>

#include "../src/Sphere.h"
#include "../src/ray.h"

TEST_CASE("A Hit")
{
    Sphere sphere(point3(0, 0, -5), 1.0);

    ray r(
        point3(0, 0, 0),
        vec3(0, 0, -1)
    );

    REQUIRE(sphere.intersect(r) == true);
}

TEST_CASE("A Miss")
{
    Sphere sphere(point3(0, 0, -5), 1.0);

    ray r(
        point3(0, 0, 0),
        vec3(0, 2, -1)
    );

    REQUIRE(sphere.intersect(r) == false);
}
