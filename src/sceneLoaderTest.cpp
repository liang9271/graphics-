#include "SceneLoader.h"
#include "SceneParser_JSON.h"

#include <iostream>
#include <memory>
#include <stdexcept>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: sceneLoaderTest <scene.json>" << std::endl;
        return 2;
    }

    try {
        SceneContainer sceneContainer;
        auto loader = std::make_shared<SceneLoader>(sceneContainer);
        SceneParser_JSON parser(loader);
        parser.parseFileData(argv[1]);

        if (!sceneContainer.getCamera().has_value()) {
            throw std::runtime_error("The scene file does not define a camera");
        }

        std::cout << "Loaded scene: "
                  << sceneContainer.getScene().getShapeCount()
                  << " shapes, "
                  << sceneContainer.getScene().getPointLights().size()
                  << " point lights, and 1 camera." << std::endl;
    } catch (const std::exception& error) {
        std::cerr << "Failed to load scene: " << error.what() << std::endl;
        return 1;
    }

    return 0;
}
