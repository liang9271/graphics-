#pragma once

#include "ISceneLoader.h"
#include "SceneContainer.h"

#include <memory>
#include <string>
#include <unordered_map>

class SceneLoader : public ISceneLoader {
public:
    explicit SceneLoader(SceneContainer& sceneContainer);

    void reserveCameras(size_t count) override;
    void reserveLights(size_t count) override;
    void reserveShapes(size_t count) override;
    void reserveShaders(size_t count) override;
    void reserveTextures(size_t count) override;

    void setSceneParams(const SceneParams& sceneParams) override;
    void addCamera(const std::string& name, const std::string& type,
                   const vec& position, const vec& viewDirection,
                   float focalLength, float imagePlaneWidth) override;
    void addPointLight(const vec& position, const vec& intensity) override;
    void addAreaLight(const vec& position, const vec& color,
                      const vec& normal, float width, float length) override;
    void addShader(const ShaderDesc& shaderDesc) override;
    void addShape(const ShapeDesc& shapeDesc) override;
    void addTexture(const std::string& type, const std::string& name,
                    const std::string& sourceFile) override;

private:
    static point3 toPoint(const vec& value);
    static color toColor(const vec& value);
    static point3 transformPoint(const glm::mat4& transform,
                                 const point3& point);

    SceneContainer& sceneContainer;
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
};
