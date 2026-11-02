#pragma once

#include "Shape.h"
#include "color.h"
#include <memory>
#include <optional>
#include <cstddef>
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
    bool intersectBruteForce(const ray& r, HitSphere& closestHit,
                             std::shared_ptr<Shape>& hitShape) const;
    void buildBVH() const;
    void setBVHEnabled(bool enabled);

private:
    struct BVHNode {
        BoundingBox bounds;
        std::size_t begin = 0;
        std::size_t count = 0;
        std::size_t left = 0;
        std::size_t right = 0;
    };

    std::size_t buildBVHNode(std::size_t begin, std::size_t end) const;
    bool intersectBVHNode(std::size_t nodeIndex, const ray& r,
                          HitSphere& closestHit,
                          std::shared_ptr<Shape>& hitShape,
                          double& closestT) const;

    std::vector<std::shared_ptr<Shape>> shapes;
    std::vector<PointLight> pointLights;
    std::optional<color> backgroundColor;
    mutable std::vector<std::size_t> bvhShapeIndices;
    mutable std::vector<BVHNode> bvhNodes;
    mutable bool bvhDirty = true;
    bool bvhEnabled = true;
};
