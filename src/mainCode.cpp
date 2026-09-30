#include "vec3.h"
#include "Framebuffer.h"
#include<iostream>
#include "ray.h"
#include "color.h"
#include "Sphere.h"
#include "LambertianShader.h"
#include "BlinnPhongShader.h"
#include <memory>

int main(int argc, char** argv) {
    
Framebuffer fb(200, 200);

  //camera delaration
    point3 camera_center(0,0,0);

//sphere delaration
    Sphere lambertianSphere(point3(-1.2, 0, -5), 1.0);
    Sphere blinnPhongSphere(point3(1.2, 0, -5), 1.0);


auto lambertian = std::make_shared<LambertianShader>(
    color(0.8, 0.2, 0.2));

lambertianSphere.setShader(lambertian);

auto blinnPhong = std::make_shared<BlinnPhongShader>(
    color(0.2, 0.2, 0.8), 
    color(1.0, 1.0, 1.0),
    32.0);

blinnPhongSphere.setShader(blinnPhong);


    //go through pixels horizontally
    for (int x = 0; x < fb.getwidth(); ++x) {
      //go through pixels vertically  
        for (int y = 0; y < fb.getHeight(); ++y) {
        
            point3 pixel_position(
                2.0 * x / fb.getwidth() - 1.0,
                2.0 * y / fb.getHeight() -1.0, 
                -1.5
            );

            //the direction of the ray from the camera center through the pixel
            vec3 direction = pixel_position - camera_center;
            // create a ray
            ray r(camera_center, direction);

            //check if its hit the sphere
HitSphere lambertianHit;
HitSphere blinnPhongHit;

bool hitLambertian = lambertianSphere.intersect(r, lambertianHit);
bool hitBlinnPhong = blinnPhongSphere.intersect(r, blinnPhongHit);

if (hitLambertian && hitBlinnPhong) {

    // Both spheres were hit.
    // Use the sphere that is closer to the camera.
    if (lambertianHit.t < blinnPhongHit.t) {
        color pixelColor =
            lambertianSphere.getShader()->rayColor(lambertianHit);

        fb.setPixelColor(x, y, pixelColor);
    }
    else {
        color pixelColor =
            blinnPhongSphere.getShader()->rayColor(blinnPhongHit);

        fb.setPixelColor(x, y, pixelColor);
    }
}
else if (hitLambertian) {

    color pixelColor =
        lambertianSphere.getShader()->rayColor(lambertianHit);

    fb.setPixelColor(x, y, pixelColor);
}
else if (hitBlinnPhong) {

    color pixelColor =
        blinnPhongSphere.getShader()->rayColor(blinnPhongHit);

    fb.setPixelColor(x, y, pixelColor);
}
else {

    double t = static_cast<double>(y) / fb.getHeight();

    color top(0.8, 0.2, 1.0);
    color bottom(0.1, 0.5, 1.0);

    color background = (1.0 - t) * top + t * bottom;

    fb.setPixelColor(x, y, background);
}
        } // y loop 
    } // x loop
    fb.exportToPNG("test_lerp.png");
} // main