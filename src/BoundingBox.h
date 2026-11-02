#pragma once

#include "ray.h"

#include <algorithm>
#include <limits>

class BoundingBox {
public:
    BoundingBox()
        : minimum(std::numeric_limits<double>::infinity(),
                  std::numeric_limits<double>::infinity(),
                  std::numeric_limits<double>::infinity()),
          maximum(-std::numeric_limits<double>::infinity(),
                  -std::numeric_limits<double>::infinity(),
                  -std::numeric_limits<double>::infinity())
    {
    }

    BoundingBox(const point3& minimum, const point3& maximum)
        : minimum(minimum), maximum(maximum)
    {
    }

    const point3& min() const
    {
        return minimum;
    }

    const point3& max() const
    {
        return maximum;
    }

    point3 centroid() const
    {
        return 0.5 * (minimum + maximum);
    }

    void include(const BoundingBox& other)
    {
        for (int axis = 0; axis < 3; ++axis) {
            minimum[axis] = std::min(minimum[axis], other.minimum[axis]);
            maximum[axis] = std::max(maximum[axis], other.maximum[axis]);
        }
    }

    bool intersects(const ray& r, double tMin, double tMax) const
    {
        for (int axis = 0; axis < 3; ++axis) {
            const double direction = r.direction()[axis];
            const double origin = r.origin()[axis];
            if (direction == 0.0) {
                if (origin < minimum[axis] || origin > maximum[axis]) {
                    return false;
                }
                continue;
            }

            const double inverseDirection = 1.0 / direction;
            double near = (minimum[axis] - origin) * inverseDirection;
            double far = (maximum[axis] - origin) * inverseDirection;
            if (near > far) {
                std::swap(near, far);
            }
            tMin = std::max(tMin, near);
            tMax = std::min(tMax, far);
            if (tMax < tMin) {
                return false;
            }
        }
        return true;
    }

private:
    point3 minimum;
    point3 maximum;
};
