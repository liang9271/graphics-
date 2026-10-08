#include "Framebuffer.h"
#include "color.h"
#include "Sphere.h"
#include "Triangle.h"
#include "LambertianShader.h"
#include "BlinnPhongShader.h"
#include "Scene.h"
#include "Renderer.h"
#include "MirrorShader.h"
#include "Camera.h"

#include <memory>

int main(int argc, char** argv)
{
    // Create the framebuffer
    Framebuffer fb(200, 200);

    // Create the scene
    Scene scene;

    // Create shaders
    auto lambertian = std::make_shared<LambertianShader>(
        color(0.8, 0.2, 0.2));

    auto blinnPhong = std::make_shared<BlinnPhongShader>(
        color(0.2, 0.2, 0.8),
        color(1.0, 1.0, 1.0),
        32.0);

    auto mirror = std::make_shared<MirrorShader>();

    // Create spheres
    auto lambertianSphere =
        std::make_shared<Sphere>(
            point3(-1.2, 0, -5),
            1.0);

    auto blinnPhongSphere =
        std::make_shared<Sphere>(
            point3(1.2, 0, -5),
            1.0);
//mirror sphere
    auto mirrorSphere1 =
    std::make_shared<Sphere>(
        point3(0, 0 , -8),
        1.0);

    auto mirrorSphere2 =
    std::make_shared<Sphere>(
        point3(-3.5, 0, -5),
        1.0);

mirrorSphere1->setShader(mirror);
mirrorSphere2->setShader(mirror);

    lambertianSphere->setShader(lambertian);
    blinnPhongSphere->setShader(blinnPhong);
//triangle as ground
    auto ground = 
    std::make_shared<Triangle>(
        point3(-10, -1.0, 3), //x
        point3(10, -1.0, 3), //y
        point3(0, -1.0, -30) //z depth
    );

    auto groundShader = 
    std::make_shared<LambertianShader>(color(0.2, 0.8, 0.2));
    ground->setShader(groundShader);

    // Add objects to the scene
    scene.addShape(lambertianSphere);
    scene.addShape(blinnPhongSphere);
    scene.addShape(ground);
    scene.addShape(mirrorSphere1);
    scene.addShape(mirrorSphere2);

  Camera camera(
    point3(0, 3.0, 4.0),
    vec3(0, -1.5, -3.0),
    0.4,
    0.5
);

Renderer renderer(camera);
renderer.render(scene, fb);

    // Save the image
    fb.exportToPNG("test_lerp.png");

    return 0;
}