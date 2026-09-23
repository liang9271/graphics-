#include "vec3.h"
#include "Framebuffer.h"
#include<iostream>
#include "ray.h"
#include "color.h"
#include "Sphere.h"

int main(int argc, char** argv) {
    
Framebuffer fb(300, 300);


    fb.clearToGradient(
	vec3(0.0,0.0,1.0), 
	vec3(1.0,0.0,0.0));

    fb.exportToPNG("test_lerp.png");
}
