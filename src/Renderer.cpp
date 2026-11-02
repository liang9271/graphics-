#include "Renderer.h"
#include "SpaceBackground.h"

#include <random>
#include <limits>
#include <stdexcept>

Renderer::Renderer(const Camera& camera, int samplesPerAxis)
    : camera(camera),
      samplesPerAxis(samplesPerAxis)
{
    if (samplesPerAxis <= 0) {
        throw std::invalid_argument(
            "Renderer samples per axis must be positive");
    }
}

void Renderer::render(
    const Scene& scene,
    Framebuffer& framebuffer)
{
    std::mt19937 generator(0);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    double aspectRatio =
        static_cast<double>(framebuffer.getwidth()) /
        framebuffer.getHeight();
    for (int x = 0; x < framebuffer.getwidth(); ++x) {
        for (int y = 0; y < framebuffer.getHeight(); ++y) {
        
          color c(0.0,0.0,0.0);        

            for (int p = 0; p < samplesPerAxis; ++p) {
                for (int q = 0; q < samplesPerAxis; ++q) {
                    float tmin = 1.0;
                    float tmax =
                        std::numeric_limits<float>::infinity();
                    float poffset =
                        (p + distribution(generator))
                        / samplesPerAxis;
                    float qoffset =
                        (q + distribution(generator))
                        / samplesPerAxis;
                    double u =
                        (x + poffset) /
                        framebuffer.getwidth();
                    double v =
                        (y + qoffset) /
                        framebuffer.getHeight();
                    ray r = camera.getRay(u,v,aspectRatio);

                    HitSphere hit;
                    std::shared_ptr<Shape> hitShape;

                    if (scene.intersect(
                            r,
                            hit,
                            hitShape)) {

                        c += hitShape->getShader()->rayColor(
                            hit,
                            scene,
                            5
                        );
                    }
                    else {
                        c += scene.getBackgroundColor().value_or(
                            spaceBackground(r.direction()));
                    }
                }
            }

            // average the accumulated color values
            c =
                c / (samplesPerAxis * samplesPerAxis);

            framebuffer.setPixelColor(
                x,
                y,
                c
            );
        }
    }
}