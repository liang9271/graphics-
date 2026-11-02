#include "Scene.h"

#include <algorithm>
#include <numeric>

void Scene::addShape(std::shared_ptr<Shape> shape)
{
    shapes.push_back(shape);
    bvhDirty = true;
}

std::size_t Scene::getShapeCount() const
{
    return shapes.size();
}

void Scene::addPointLight(const point3& position, const color& intensity)
{
    pointLights.push_back({position, intensity});
}

const std::vector<Scene::PointLight>& Scene::getPointLights() const
{
    return pointLights;
}

void Scene::setBackgroundColor(const color& backgroundColor)
{
    this->backgroundColor = backgroundColor;
}

const std::optional<color>& Scene::getBackgroundColor() const
{
    return backgroundColor;
}

bool Scene::intersect(const ray& r,
                      HitSphere& closestHit,
                      std::shared_ptr<Shape>& hitShape) const
{
    if (!bvhEnabled) {
        return intersectBruteForce(r, closestHit, hitShape);
    }
    buildBVH();
    if (bvhNodes.empty()) {
        return false;
    }

    double closestT = 1e30;
    return intersectBVHNode(0, r, closestHit, hitShape, closestT);
}

bool Scene::intersectBruteForce(const ray& r,
                                HitSphere& closestHit,
                                std::shared_ptr<Shape>& hitShape) const
{
    bool hitAnything = false;
    double closestT = 1e30;

    for (const auto& shape : shapes) {

        HitSphere tempHit;

        if (shape->intersect(r, tempHit)) {

            // Ignore intersections behind the camera.
            if (tempHit.t < 0) {
                continue;
            }

            // Keep the closest object.
            if (tempHit.t < closestT) {
                closestT = tempHit.t;
                closestHit = tempHit;
                hitShape = shape;
                hitAnything = true;
            }
        }
    }

    return hitAnything;
}

void Scene::setBVHEnabled(bool enabled)
{
    bvhEnabled = enabled;
}

void Scene::buildBVH() const
{
    if (!bvhDirty) {
        return;
    }

    bvhShapeIndices.resize(shapes.size());
    std::iota(bvhShapeIndices.begin(), bvhShapeIndices.end(), 0);
    bvhNodes.clear();
    bvhNodes.reserve(shapes.size() * 2);
    if (!shapes.empty()) {
        buildBVHNode(0, shapes.size());
    }
    bvhDirty = false;
}

std::size_t Scene::buildBVHNode(std::size_t begin, std::size_t end) const
{
    const std::size_t nodeIndex = bvhNodes.size();
    bvhNodes.emplace_back();

    BoundingBox bounds;
    BoundingBox centroidBounds;
    for (std::size_t i = begin; i < end; ++i) {
        const BoundingBox shapeBounds =
            shapes[bvhShapeIndices[i]]->boundingBox();
        bounds.include(shapeBounds);
        centroidBounds.include(
            BoundingBox(shapeBounds.centroid(), shapeBounds.centroid()));
    }
    bvhNodes[nodeIndex].bounds = bounds;

    constexpr std::size_t leafSize = 4;
    const std::size_t count = end - begin;
    if (count <= leafSize) {
        bvhNodes[nodeIndex].begin = begin;
        bvhNodes[nodeIndex].count = count;
        return nodeIndex;
    }

    const vec3 extent = centroidBounds.max() - centroidBounds.min();
    int axis = 0;
    if (extent.y() > extent.x()) {
        axis = 1;
    }
    if (extent.z() > extent[axis]) {
        axis = 2;
    }
    const std::size_t middle = begin + count / 2;
    std::nth_element(
        bvhShapeIndices.begin() + static_cast<std::ptrdiff_t>(begin),
        bvhShapeIndices.begin() + static_cast<std::ptrdiff_t>(middle),
        bvhShapeIndices.begin() + static_cast<std::ptrdiff_t>(end),
        [this, axis](std::size_t left, std::size_t right) {
            return shapes[left]->boundingBox().centroid()[axis] <
                   shapes[right]->boundingBox().centroid()[axis];
        });

    const std::size_t left = buildBVHNode(begin, middle);
    const std::size_t right = buildBVHNode(middle, end);
    bvhNodes[nodeIndex].left = left;
    bvhNodes[nodeIndex].right = right;
    return nodeIndex;
}

bool Scene::intersectBVHNode(
    std::size_t nodeIndex,
    const ray& r,
    HitSphere& closestHit,
    std::shared_ptr<Shape>& hitShape,
    double& closestT) const
{
    const BVHNode& node = bvhNodes[nodeIndex];
    if (!node.bounds.intersects(r, 0.0, closestT)) {
        return false;
    }

    bool hitAnything = false;
    if (node.count > 0) {
        for (std::size_t i = node.begin; i < node.begin + node.count; ++i) {
            const auto& shape = shapes[bvhShapeIndices[i]];
            HitSphere candidate;
            if (shape->intersect(r, candidate) &&
                candidate.t >= 0.0 && candidate.t < closestT) {
                closestT = candidate.t;
                closestHit = candidate;
                hitShape = shape;
                hitAnything = true;
            }
        }
        return hitAnything;
    }

    hitAnything |= intersectBVHNode(
        node.left, r, closestHit, hitShape, closestT);
    hitAnything |= intersectBVHNode(
        node.right, r, closestHit, hitShape, closestT);
    return hitAnything;
}
