#pragma once

#include "Camera.h"
#include "Scene.h"

#include <optional>

class SceneContainer {
public:
    Scene& getScene();
    const Scene& getScene() const;

    std::optional<Camera>& getCamera();
    const std::optional<Camera>& getCamera() const;

private:
    Scene scene;
    std::optional<Camera> camera;
};
