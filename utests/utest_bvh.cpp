#include <catch2/catch_test_macros.hpp>

#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"

#include <memory>

TEST_CASE("BVH agrees with brute-force closest intersections")
{
    Scene scene;
    scene.addShape(
        std::make_shared<Sphere>(point3(0, 0, -8), 1.0));
    scene.addShape(
        std::make_shared<Sphere>(point3(0, 0, -4), 0.5));
    scene.addShape(std::make_shared<Triangle>(
        point3(-2, -2, -10),
        point3(2, -2, -10),
        point3(0, 2, -10)));
    for (int i = 0; i < 5; ++i) {
        scene.addShape(std::make_shared<Sphere>(
            point3(10.0 + i, 0, -5), 0.25));
    }

    const ray primaryRay(point3(0, 0, 0), vec3(0, 0, -1));
    HitSphere bvhHit;
    HitSphere bruteForceHit;
    std::shared_ptr<Shape> bvhShape;
    std::shared_ptr<Shape> bruteForceShape;

    REQUIRE(scene.intersect(primaryRay, bvhHit, bvhShape));
    REQUIRE(scene.intersectBruteForce(
        primaryRay, bruteForceHit, bruteForceShape));
    REQUIRE(bvhShape == bruteForceShape);
    REQUIRE(bvhHit.t == bruteForceHit.t);

    const ray triangleRay(point3(0, 1.5, 0), vec3(0, 0, -1));
    REQUIRE(scene.intersect(triangleRay, bvhHit, bvhShape));
    REQUIRE(scene.intersectBruteForce(
        triangleRay, bruteForceHit, bruteForceShape));
    REQUIRE(bvhShape == bruteForceShape);
    REQUIRE(bvhHit.t == bruteForceHit.t);

    scene.addShape(
        std::make_shared<Sphere>(point3(0, 0, -2), 0.25));
    REQUIRE(scene.intersect(primaryRay, bvhHit, bvhShape));
    REQUIRE(scene.intersectBruteForce(
        primaryRay, bruteForceHit, bruteForceShape));
    REQUIRE(bvhShape == bruteForceShape);
    REQUIRE(bvhHit.t == bruteForceHit.t);
}

TEST_CASE("BVH handles empty scenes and axis-parallel rays")
{
    Scene emptyScene;
    HitSphere hit;
    std::shared_ptr<Shape> shape;
    REQUIRE_FALSE(emptyScene.intersect(
        ray(point3(0, 0, 0), vec3(0, 0, -1)), hit, shape));

    Scene scene;
    scene.addShape(
        std::make_shared<Sphere>(point3(2, 0, -5), 1.0));
    const ray parallelRay(point3(0, 0, 0), vec3(0, 0, -1));
    REQUIRE_FALSE(scene.intersect(parallelRay, hit, shape));
}
