#include "SceneLoader.h"

#include "BlinnPhongShader.h"
#include "LambertianShader.h"
#include "MirrorShader.h"
#include "Sphere.h"
#include "Triangle.h"

#include <cmath>
#include <stdexcept>

Scene& SceneContainer::getScene()
{
    return scene;
}

const Scene& SceneContainer::getScene() const
{
    return scene;
}

std::optional<Camera>& SceneContainer::getCamera()
{
    return camera;
}

const std::optional<Camera>& SceneContainer::getCamera() const
{
    return camera;
}

SceneLoader::SceneLoader(SceneContainer& sceneContainer)
    : sceneContainer(sceneContainer)
{
}

void SceneLoader::reserveCameras(size_t)
{
}

void SceneLoader::reserveLights(size_t)
{
}

void SceneLoader::reserveShapes(size_t)
{
}

void SceneLoader::reserveShaders(size_t count)
{
    shaders.reserve(count);
}

void SceneLoader::reserveTextures(size_t)
{
}

void SceneLoader::setSceneParams(const SceneParams& sceneParams)
{
    if (!sceneParams.envMapFile.empty()) {
        throw std::runtime_error(
            "Environment maps are not supported by this renderer");
    }
    sceneContainer.getScene().setBackgroundColor(toColor(sceneParams.bgColor));
}

void SceneLoader::addCamera(
    const std::string&,
    const std::string& type,
    const vec& position,
    const vec& viewDirection,
    float focalLength,
    float imagePlaneWidth)
{
    if (type != "perspective") {
        throw std::runtime_error("Unsupported camera type: " + type);
    }

    auto& camera = sceneContainer.getCamera();
    if (camera.has_value()) {
        throw std::runtime_error(
            "This renderer supports one camera per scene");
    }

    camera.emplace(toPoint(position), toPoint(viewDirection), focalLength,
                   imagePlaneWidth);
}

void SceneLoader::addPointLight(const vec& position, const vec& intensity)
{
    sceneContainer.getScene().addPointLight(toPoint(position), toColor(intensity));
}

void SceneLoader::addAreaLight(const vec&, const vec&, const vec&, float, float)
{
    throw std::runtime_error("Area lights are not supported by this renderer");
}

void SceneLoader::addShader(const ShaderDesc& shaderDesc)
{
    std::shared_ptr<Shader> shader;
    if (shaderDesc.type == "Lambertian" || shaderDesc.type == "Diffuse") {
        shader = std::make_shared<LambertianShader>(
            toColor(shaderDesc.diffuse.data));
    } else if (shaderDesc.type == "BlinnPhong" ||
               shaderDesc.type == "Phong") {
        shader = std::make_shared<BlinnPhongShader>(
            toColor(shaderDesc.diffuse.data),
            toColor(shaderDesc.specular.data),
            shaderDesc.phongExp);
    } else if (shaderDesc.type == "Mirror") {
        shader = std::make_shared<MirrorShader>();
    } else {
        throw std::runtime_error(
            "Unsupported shader type: " + shaderDesc.type);
    }

    const auto [iterator, inserted] =
        shaders.emplace(shaderDesc.name, std::move(shader));
    if (!inserted) {
        throw std::runtime_error(
            "Duplicate shader name in scene: " + shaderDesc.name);
    }
}

void SceneLoader::addShape(const ShapeDesc& shapeDesc)
{
    const auto shader = shaders.find(shapeDesc.shaderNameReference);
    if (shader == shaders.end()) {
        throw std::runtime_error(
            "Shape references unknown shader: " +
            shapeDesc.shaderNameReference);
    }

    std::shared_ptr<Shape> shape;
    if (shapeDesc.type == "sphere") {
        point3 center = toPoint(shapeDesc.center);
        double radius = shapeDesc.radius;
        if (shapeDesc.hasXForm) {
            const glm::mat4& transform = shapeDesc.localXForm;
            const glm::vec3 scaleX(transform[0]);
            const glm::vec3 scaleY(transform[1]);
            const glm::vec3 scaleZ(transform[2]);
            const float scale = glm::length(scaleX);
            if (std::fabs(scale - glm::length(scaleY)) > 1e-5f ||
                std::fabs(scale - glm::length(scaleZ)) > 1e-5f) {
                throw std::runtime_error(
                    "Non-uniformly scaled spheres are not supported");
            }
            center = transformPoint(transform, center);
            radius *= scale;
        }
        if (radius <= 0.0) {
            throw std::runtime_error("Sphere radius must be positive");
        }
        shape = std::make_shared<Sphere>(center, radius);
    } else if (shapeDesc.type == "triangle") {
        point3 v0 = toPoint(shapeDesc.v0);
        point3 v1 = toPoint(shapeDesc.v1);
        point3 v2 = toPoint(shapeDesc.v2);
        if (shapeDesc.hasXForm) {
            v0 = transformPoint(shapeDesc.localXForm, v0);
            v1 = transformPoint(shapeDesc.localXForm, v1);
            v2 = transformPoint(shapeDesc.localXForm, v2);
        }
        shape = std::make_shared<Triangle>(v0, v1, v2);
    } else {
        throw std::runtime_error(
            "Unsupported shape type: " + shapeDesc.type);
    }

    shape->setShader(shader->second);
    sceneContainer.getScene().addShape(std::move(shape));
}

void SceneLoader::addTexture(
    const std::string& type,
    const std::string&,
    const std::string&)
{
    throw std::runtime_error("Unsupported texture type: " + type);
}

point3 SceneLoader::toPoint(const vec& value)
{
    return point3(value.x, value.y, value.z);
}

color SceneLoader::toColor(const vec& value)
{
    return color(value.x, value.y, value.z);
}

point3 SceneLoader::transformPoint(
    const glm::mat4& transform,
    const point3& point)
{
    const glm::vec4 transformed =
        transform * glm::vec4(point.x(), point.y(), point.z(), 1.0f);
    if (std::fabs(transformed.w) < 1e-8f) {
        throw std::runtime_error("Shape transform produced an invalid point");
    }
    return point3(transformed.x / transformed.w,
                  transformed.y / transformed.w,
                  transformed.z / transformed.w);
}
