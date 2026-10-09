
#include "Framebuffer.h"
#include "png++/png.hpp"
#include <algorithm>

Framebuffer::Framebuffer()
    : width(100), height(100), fbStorage(width * height) {}

Framebuffer::Framebuffer(int width, int height)
    : width(width), height(height), fbStorage(width * height) {}

void Framebuffer::clearToColor(const color& c) {
    for (auto idx = 0; idx < fbStorage.size(); idx++) {
        setPixelColor(idx, c);
    }
}

void Framebuffer::clearToGradient(const color& c1, const color& c2) {
    for (auto x = 0; x < width; x++) {
        for (auto y = 0; y < height; y++) {
            auto t = double(y) / height;
            color c = (1 - t) * c1 + t * c2;
            setPixelColor(x, y, c);
        }
    }
}

void Framebuffer::setPixelColor(int i, int j, const color& c) {
    fbStorage[j * width + i] = c;
}

void Framebuffer::setPixelColor(int idx, const color& c) {
    fbStorage[idx] = c;
}

void Framebuffer::exportToPNG(const std::string& filename) {
    png::image<png::rgb_pixel> imData(width, height);

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            int flippedj = height - 1 - j;
            vec3 pixelColor = fbStorage[flippedj * width + i];

            png::byte r = static_cast<png::byte>(
                std::clamp(pixelColor.x(), 0.0, 1.0) * 255.0);
            png::byte g = static_cast<png::byte>(
                std::clamp(pixelColor.y(), 0.0, 1.0) * 255.0);
            png::byte b = static_cast<png::byte>(
                std::clamp(pixelColor.z(), 0.0, 1.0) * 255.0);

            imData[j][i] = png::rgb_pixel(r, g, b);
        }
    }

    imData.write(filename);
}

int Framebuffer::getwidth() const {
    return width;
}

int Framebuffer::getHeight() const {
    return height;
}

void Framebuffer::getRGBData(std::vector<unsigned char>& data) const {
    data.resize(width * height * 3);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const color& c = fbStorage[(height - 1 - y) * width + x];
            int idx = (y * width + x) * 3;

            data[idx] = static_cast<unsigned char>(
                std::clamp(c.x(), 0.0, 1.0) * 255.0);
            data[idx + 1] = static_cast<unsigned char>(
                std::clamp(c.y(), 0.0, 1.0) * 255.0);
            data[idx + 2] = static_cast<unsigned char>(
                std::clamp(c.z(), 0.0, 1.0) * 255.0);
        }
    }
}