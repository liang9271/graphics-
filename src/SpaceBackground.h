#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

#include "color.h"

inline color spaceBackground(const vec3& direction) {
    const vec3 unitDirection = unit_vector(direction);
    const double vertical = 0.5 * (unitDirection.y() + 1.0);
    const color top(0.006, 0.004, 0.025);
    const color bottom(0.025, 0.025, 0.10);
    color background = (1.0 - vertical) * top + vertical * bottom;

    constexpr double pi = 3.14159265358979323846;
    constexpr int longitudeCells = 720;
    constexpr int latitudeCells = 360;
    const double longitude =
        (std::atan2(unitDirection.z(), unitDirection.x()) + pi) /
        (2.0 * pi);
    const double latitude =
        (std::asin(std::clamp(unitDirection.y(), -1.0, 1.0)) + pi / 2.0) /
        pi;
    const double cellX = longitude * longitudeCells;
    const double cellY = latitude * latitudeCells;
    const int x = static_cast<int>(cellX);
    const int y = static_cast<int>(cellY);

    std::uint32_t hash =
        static_cast<std::uint32_t>(x) * 0x9e3779b9u ^
        static_cast<std::uint32_t>(y) * 0x85ebca6bu;
    hash ^= hash >> 16;
    hash *= 0x7feb352du;
    hash ^= hash >> 15;

    if (hash % 13u == 0u) {
        const double starX = ((hash >> 8) & 0xffffu) / 65536.0;
        const double starY = ((hash >> 16) & 0xffffu) / 65536.0;
        const double dx = (cellX - x) - starX;
        const double dy = (cellY - y) - starY;
        const double distanceSquared = dx * dx + dy * dy;
        const double radius = 0.42;
        if (distanceSquared < radius * radius) {
            const double brightness =
                (0.55 + (hash % 100u) / 220.0) *
                (1.0 - std::sqrt(distanceSquared) / radius);
            background += color(
                brightness * 0.78,
                brightness * 0.86,
                brightness);
        }
    }

    return background;
}
