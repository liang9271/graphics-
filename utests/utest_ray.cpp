#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "ray.h"

using Catch::Matchers::WithinAbs;

TEST_CASE("Ray construction stores origin and direction")
{
    point3 origin(1.0, 2.0, 3.0);
    vec3 direction(4.0, 5.0, 6.0);

    ray r(origin, direction);

    REQUIRE_THAT(r.origin().x(), WithinAbs(1.0, 0.0001));
    REQUIRE_THAT(r.origin().y(), WithinAbs(2.0, 0.0001));
    REQUIRE_THAT(r.origin().z(), WithinAbs(3.0, 0.0001));

    REQUIRE_THAT(r.direction().x(), WithinAbs(4.0, 0.0001));
    REQUIRE_THAT(r.direction().y(), WithinAbs(5.0, 0.0001));
    REQUIRE_THAT(r.direction().z(), WithinAbs(6.0, 0.0001));
}

TEST_CASE("Ray at t = 0 returns the origin")
{
    ray r(point3(1.0, 2.0, 3.0), vec3(4.0, 5.0, 6.0));

    point3 p = r.at(0.0);

    REQUIRE_THAT(p.x(), WithinAbs(1.0, 0.0001));
    REQUIRE_THAT(p.y(), WithinAbs(2.0, 0.0001));
    REQUIRE_THAT(p.z(), WithinAbs(3.0, 0.0001));
}

TEST_CASE("Ray at t = 1 returns origin plus direction")
{
    ray r(point3(1.0, 2.0, 3.0), vec3(4.0, 5.0, 6.0));

    point3 p = r.at(1.0);

    REQUIRE_THAT(p.x(), WithinAbs(5.0, 0.0001));
    REQUIRE_THAT(p.y(), WithinAbs(7.0, 0.0001));
    REQUIRE_THAT(p.z(), WithinAbs(9.0, 0.0001));
}

TEST_CASE("Ray at positive t")
{
    ray r(point3(9.2, 1.0, 2.3), vec3(3.5, 5.0, 2.0));

    point3 p = r.at(0.5);

    REQUIRE_THAT(p.x(), WithinAbs(10.95, 0.0001));
    REQUIRE_THAT(p.y(), WithinAbs(3.5, 0.0001));
    REQUIRE_THAT(p.z(), WithinAbs(3.3, 0.0001));
}

TEST_CASE("Ray at negative t")
{
    ray r(point3(9.2, 1.0, 2.3), vec3(3.5, 5.0, 2.0));

    point3 p = r.at(-1.0);

    REQUIRE_THAT(p.x(), WithinAbs(5.7, 0.0001));
    REQUIRE_THAT(p.y(), WithinAbs(-4.0, 0.0001));
    REQUIRE_THAT(p.z(), WithinAbs(0.3, 0.0001));
}

TEST_CASE("Ray does not change after evaluating at")
{
    point3 origin(1.0, 2.0, 3.0);
    vec3 direction(4.0, 5.0, 6.0);

    ray r(origin, direction);

    r.at(2.0);
    r.at(-3.0);

    REQUIRE_THAT(r.origin().x(), WithinAbs(1.0, 0.0001));
    REQUIRE_THAT(r.origin().y(), WithinAbs(2.0, 0.0001));
    REQUIRE_THAT(r.origin().z(), WithinAbs(3.0, 0.0001));

    REQUIRE_THAT(r.direction().x(), WithinAbs(4.0, 0.0001));
    REQUIRE_THAT(r.direction().y(), WithinAbs(5.0, 0.0001));
    REQUIRE_THAT(r.direction().z(), WithinAbs(6.0, 0.0001));
}
