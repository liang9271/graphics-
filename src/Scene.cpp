#include "Scene.h"

void Scene::addShape(std::shared_ptr<Shape> shape)
{
    shapes.push_back(shape);
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
