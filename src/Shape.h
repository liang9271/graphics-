#pragma once
#include "ray.h"

class Shape {
public:

virtual bool intersect(const ray& r) = 0;
virtual ~Shape() =  default;

};
