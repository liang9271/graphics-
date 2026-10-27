#pragma once

#include "Shape.h"
#include "color.h"
#include <memory>
#include <optional>
#include <vector>

class Scene {
public:
    struct PointLight {
        point3 position;
        color intensity;
    };

    void addShape(std::shared_ptr<Shape> shape);
    std::size_t getShapeCount() const;
    void addPointLight(const point3& position, const color& intensity);
    const std::vector<PointLight>& getPointLights() const;
    void setBackgroundColor(const color& backgroundColor);
    const std::optional<color>& getBackgroundColor() const;

    bool intersect(const ray& r, HitSphere& closestHit,
                   std::shared_ptr<Shape>& hitShape) const;

private:
    std::vector<std::shared_ptr<Shape>> shapes;
    std::vector<PointLight> pointLights;
    std::optional<color> backgroundColor;
};
