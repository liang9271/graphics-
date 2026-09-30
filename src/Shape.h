#pragma once
#include "ray.h"
#include "Shader.h"
#include <memory>

class Shader;

struct HitSphere {
    point3 p;
    vec3 normal;
    double t;
    ray r;
};

class Shape {
    protected: 
    std::shared_ptr<Shader> shader;

public:

    virtual bool intersect(const ray& r, HitSphere& hit) = 0;

    virtual ~Shape() = default;

    void setShader(std::shared_ptr<Shader> s) {
        shader = s;
    }

    std::shared_ptr<Shader> getShader() const {
        return shader;
    }

};
