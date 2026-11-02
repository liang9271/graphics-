#pragma once

#include "Framebuffer.h"
#include "Scene.h"
#include "Camera.h"

class Renderer {
public:
    Renderer(const Camera& camera, int samplesPerAxis = 4);

    void render(
        const Scene& scene,
        Framebuffer& framebuffer
    );

private:
    Camera camera;
    int samplesPerAxis;
};