#include "Renderer.h"

#include <random>
#include <limits>

Renderer::Renderer(const Camera& camera)
    : camera(camera)
{
}

void Renderer::render(
    const Scene& scene,
    Framebuffer& framebuffer)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    int rpp_NSquare = 4;

    double aspectRatio =
        static_cast<double>(framebuffer.getwidth()) /
        framebuffer.getHeight();
    for (int x = 0; x < framebuffer.getwidth(); ++x) {
        for (int y = 0; y < framebuffer.getHeight(); ++y) {
        
          color c(0.0,0.0,0.0);        

            for (int p = 0; p < rpp_NSquare; ++p) {
                for (int q = 0; q < rpp_NSquare; ++q) {
                    float tmin = 1.0;
                    float tmax =
                        std::numeric_limits<float>::infinity();
                    float poffset =
                        (p + distribution(generator))
                        / rpp_NSquare;
                    float qoffset =
                        (q + distribution(generator))
                        / rpp_NSquare;
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

                        double t = v;

                        color top(0.8, 0.2, 1.0);
                        color bottom(0.1, 0.5, 1.0);

                        c +=
                            (1.0 - t) * top +
                            t * bottom;
                    }
                }
            }

            // average the accumulated color values
            c =
                c / (rpp_NSquare * rpp_NSquare);

            framebuffer.setPixelColor(
                x,
                y,
                c
            );
        }
    }
}