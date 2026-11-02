#include "Camera.h"
#include "Framebuffer.h"
#include "LambertianShader.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;

double milliseconds(Clock::duration duration)
{
    return std::chrono::duration<double, std::milli>(duration).count();
}

void addBenchmarkScene(Scene& scene)
{
    constexpr int columns = 12;
    constexpr int rows = 9;
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            const double radius = 0.25 + 0.02 * ((row + column) % 5);
            const point3 center(
                (column - (columns - 1) / 2.0) * 0.8,
                (row - (rows - 1) / 2.0) * 0.8,
                -8.0);
            auto sphere = std::make_shared<Sphere>(center, radius);
            const color diffuse(
                0.25 + 0.06 * (column % 8),
                0.25 + 0.07 * (row % 7),
                0.5);
            sphere->setShader(std::make_shared<LambertianShader>(diffuse));
            scene.addShape(std::move(sphere));
        }
    }

    auto ground = std::make_shared<Triangle>(
        point3(-30, -4, -30),
        point3(30, -4, -30),
        point3(0, -4, -50));
    ground->setShader(
        std::make_shared<LambertianShader>(color(0.65, 0.65, 0.65)));
    scene.addShape(std::move(ground));
    scene.addPointLight(point3(-4, 7, 1), color(1, 1, 1));
    scene.setBackgroundColor(color(0.08, 0.12, 0.2));
}

} // namespace

int main()
{
    constexpr int width = 1000;
    constexpr int height = 1000;
    constexpr int samplesPerPixel = 4;

    Scene scene;
    addBenchmarkScene(scene);
    const Camera camera(
        point3(0, 0, 0), vec3(0, 0, -1), 1.0, 1.5);
    Renderer renderer(camera, 2);
    Framebuffer bvhFramebuffer(width, height);
    Framebuffer bruteForceFramebuffer(width, height);
    constexpr std::size_t repetitions = 3;

    const auto buildStart = Clock::now();
    scene.buildBVH();
    const double buildTime = milliseconds(Clock::now() - buildStart);

    std::array<double, repetitions> bvhTimes;
    std::array<double, repetitions> bruteForceTimes;
    for (std::size_t repetition = 0; repetition < repetitions; ++repetition) {
        scene.setBVHEnabled(true);
        const auto bvhStart = Clock::now();
        renderer.render(scene, bvhFramebuffer);
        bvhTimes[repetition] = milliseconds(Clock::now() - bvhStart);

        scene.setBVHEnabled(false);
        const auto bruteForceStart = Clock::now();
        renderer.render(scene, bruteForceFramebuffer);
        bruteForceTimes[repetition] =
            milliseconds(Clock::now() - bruteForceStart);

        std::vector<unsigned char> bvhImage;
        std::vector<unsigned char> bruteForceImage;
        bvhFramebuffer.getRGBData(bvhImage);
        bruteForceFramebuffer.getRGBData(bruteForceImage);
        if (bvhImage != bruteForceImage) {
            std::cerr
                << "BVH and brute-force renders produced different images."
                << std::endl;
            return 1;
        }
    }
    std::sort(bvhTimes.begin(), bvhTimes.end());
    std::sort(bruteForceTimes.begin(), bruteForceTimes.end());
    const double bvhMedian = bvhTimes[repetitions / 2];
    const double bruteForceMedian = bruteForceTimes[repetitions / 2];

    std::cout << std::fixed << std::setprecision(2)
              << "Scene: 108 Lambertian spheres and 1 triangle\n"
              << "Image: " << width << "x" << height << ", "
              << samplesPerPixel << " rays per pixel\n"
              << "BVH build: " << buildTime << " ms\n"
              << "Median BVH render (" << repetitions << " runs): "
              << bvhMedian << " ms\n"
              << "Median brute-force render (" << repetitions << " runs): "
              << bruteForceMedian << " ms\n"
              << "Speedup: " << bruteForceMedian / bvhMedian << "x\n"
              << "Image comparison: identical\n";
    return 0;
}
