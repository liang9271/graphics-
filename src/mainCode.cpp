#include "vec3.h"
#include "Framebuffer.h"
#include<iostream>
#include "ray.h"
#include "color.h"
#include "Sphere.h"

int main(int argc, char** argv) {
    
Framebuffer fb(300, 300);

  //camera delaration
    point3 camera_center(0,0,0);

//sphere delaration
   Sphere sphere(point3(0,0,-5), 1.0);



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
            if (sphere.intersect(r)) {

                color color0(0.1, 0.3, 1.0); // blue
                // set the pixel to white if it intersects the sphere
                fb.setPixelColor(x, y, color0 ); // set the pixel to blue if it intersects the sphere
            }
            else {

                double t = static_cast<double>(y) / fb.getHeight();
                color top(0.8, 0.2, 1.0);
                color bottom(0.1, 0.5, 1.0);
                color background = (1.0 - t) * top + t * bottom;
                // set the pixel to black if it does not intersect the sphere
                fb.setPixelColor(x, y, background); // set the pixel to black if it does not intersect the sphere
            }
        }
    }   
    //save the image
    fb.exportToPNG("test_lerp.png");
}
