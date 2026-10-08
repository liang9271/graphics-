#include "Camera.h"

Camera::Camera(
    const point3& position,
    const vec3& viewDirection,
    double focalLength,
    double imagePlaneWidth)
    : position(position),
      forward(unit_vector(viewDirection)),
      focalLength(focalLength),
      imagePlaneWidth(imagePlaneWidth)
{
    // World up direction.
    vec3 worldUp(0, 1, 0);

    // Calculate the camera's right direction.
    right = unit_vector(cross(forward, worldUp));

    // Calculate the camera's up direction.
    up = unit_vector(cross(right, forward));
}

ray Camera::getRay(
    double u,
    double v,
    double aspectRatio) const
{
    // Calculate image plane height from the aspect ratio.
    double imagePlaneHeight =
        imagePlaneWidth / aspectRatio;

    // Convert [0,1] coordinates to coordinates
    // centered around the middle of the image.
    double x =
        (u - 0.5) * imagePlaneWidth;

    double y =
        (v - 0.5) * imagePlaneHeight;

    // Find the sampled position on the image plane.
    point3 pixelPosition =
        position
        + focalLength * forward
        + x * right
        + y * up;

    // Ray starts at the camera and goes
    // through the sampled image-plane position.
    vec3 direction =
        pixelPosition - position;

    return ray(position, direction);
}