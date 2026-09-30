#pragma once
#include "vec3.h"
#include "ray.h"

class Shape;

struct HitSturcture {

double t; //how far along the ray
vec3 point; //where the ray hit
vec3 normal; //direction of the surface
ray r; //original ray
Shape* shape; //which shape was hit

};
