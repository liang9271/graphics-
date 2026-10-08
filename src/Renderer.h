#pragma once

#include "Framebuffer.h"
#include "Scene.h"
#include "Camera.h"

class Renderer {
public:
    Renderer(const Camera& camera);

    void render(
        const Scene& scene,
        Framebuffer& framebuffer
    );

private:
    Camera camera;
};