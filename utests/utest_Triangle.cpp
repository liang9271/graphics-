#include <catch2/catch_test_macros.hpp>
#include "Triangle.h"

TEST_CASE("Triangle hit")
{
    Triangle triangle(
        point3(-1, -1, -5),
        point3(1, -1, -5),
        point3(0, 1, -5)
    );

    ray r(
        point3(0, 0, 0),
        vec3(0, 0, -1)
    );

    HitSphere hit;

    REQUIRE(triangle.intersect(r, hit) == true);
}

TEST_CASE("Triangle miss")
{
    Triangle triangle(
        point3(-1, -1, -5),
        point3(1, -1, -5),
        point3(0, 1, -5)
    );

    ray r(
        point3(3, 0, 0),
        vec3(0, 0, -1)
    );

    HitSphere hit;

    REQUIRE(triangle.intersect(r, hit) == false);
}
