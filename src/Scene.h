#pragma once

#include "Shape.h"
#include <memory>
#include <vector>

class Scene {
public:
    void addShape(std::shared_ptr<Shape> shape);

    bool intersect(const ray& r, HitSphere& closestHit,
                   std::shared_ptr<Shape>& hitShape) const;

private:
    std::vector<std::shared_ptr<Shape>> shapes;
};
