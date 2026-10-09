#include "Framebuffer.h"
#include "color.h"
#include "Sphere.h"
#include "Triangle.h"
#include "LambertianShader.h"
#include "BlinnPhongShader.h"
#include "Scene.h"
#include "Renderer.h"
#include "MirrorShader.h"
#include "Camera.h"
#include "handleGraphicsArgs.h"

#include <iostream>
#include <memory>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace {

GLuint compileShader(GLenum type, const char* source) {
    const GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(static_cast<std::size_t>(logLength));
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        std::cerr << "OpenGL shader compilation failed: " << log.data()
                  << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createDisplayProgram() {
    constexpr const char* vertexSource = R"(
        #version 330 core
        layout (location = 0) in vec2 position;
        layout (location = 1) in vec2 textureCoordinate;
        out vec2 uv;
        void main() {
            gl_Position = vec4(position, 0.0, 1.0);
            uv = textureCoordinate;
        }
    )";
    constexpr const char* fragmentSource = R"(
        #version 330 core
        in vec2 uv;
        out vec4 fragmentColor;
        uniform sampler2D renderedImage;
        void main() {
            fragmentColor = texture(renderedImage, uv);
        }
    )";

    const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    if (vertexShader == 0) {
        return 0;
    }
    const GLuint fragmentShader =
        compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);
        return 0;
    }

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(static_cast<std::size_t>(logLength));
        glGetProgramInfoLog(program, logLength, nullptr, log.data());
        std::cerr << "OpenGL display program linking failed: " << log.data()
                  << std::endl;
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

} // namespace

int main(int argc, char** argv)
{
    // Create the framebuffer
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    Framebuffer fb(args.width, args.height);

    // Create the scene
    Scene scene;

    // Create shaders
    auto lambertian = std::make_shared<LambertianShader>(
        color(0.8, 0.2, 0.2));

    auto blinnPhong = std::make_shared<BlinnPhongShader>(
        color(0.2, 0.2, 0.8),
        color(1.0, 1.0, 1.0),
        32.0);

    auto mirror = std::make_shared<MirrorShader>();

    // Create spheres
    auto lambertianSphere =
        std::make_shared<Sphere>(
            point3(-1.2, 0, -5),
            1.0);

    auto blinnPhongSphere =
        std::make_shared<Sphere>(
            point3(1.2, 0, -5),
            1.0);
//mirror sphere
    auto mirrorSphere1 =
    std::make_shared<Sphere>(
        point3(0, 0 , -8),
        1.0);

    auto mirrorSphere2 =
    std::make_shared<Sphere>(
        point3(-3.5, 0, -5),
        1.0);

    mirrorSphere1->setShader(mirror);
    mirrorSphere2->setShader(mirror);

    lambertianSphere->setShader(lambertian);
    blinnPhongSphere->setShader(blinnPhong);

//triangle as ground
    auto ground = 
    std::make_shared<Triangle>(
        point3(-10, -1.0, 3), //x
        point3(10, -1.0, 3), //y
        point3(0, -1.0, -30) //z depth
    );

    auto groundShader = 
    std::make_shared<LambertianShader>(color(0.75, 0.75, 0.75));
    ground->setShader(groundShader);

    // Add objects to the scene
    scene.addShape(lambertianSphere);
    scene.addShape(blinnPhongSphere);
    scene.addShape(ground);
    scene.addShape(mirrorSphere1);
    scene.addShape(mirrorSphere2);

  Camera camera(
    point3(-1, 3.0, 1.0),
    vec3(0, -1.5, -3.0),
    0.4,
    0.5
);

Renderer renderer(camera);
renderer.render(scene, fb);

    // Save the image and change the size of window
    const std::string filename = args.outputFileName.empty()
        ? "test_lerp.png"
        : args.outputFileName;
    fb.exportToPNG(filename);

    if (glfwInit() == GLFW_FALSE) {
        std::cerr << "Could not initialize GLFW." << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    GLFWwindow* window =
        glfwCreateWindow(800, 800, "Ray Tracer Live View", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Could not create the live preview window." << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Could not initialize GLEW." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
    glGetError();

    const GLuint program = createDisplayProgram();
    if (program == 0) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    constexpr GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 0.0f
    };
    GLuint vertexArray = 0;
    GLuint vertexBuffer = 0;
    GLuint texture = 0;
    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);
    glGenTextures(1, &texture);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          reinterpret_cast<void*>(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    std::vector<unsigned char> rgbData;
    fb.getRGBData(rgbData);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, fb.getwidth(), fb.getHeight(), 0,
                 GL_RGB, GL_UNSIGNED_BYTE, rgbData.data());

    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "renderedImage"), 0);

    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        int windowWidth = 0;
        int windowHeight = 0;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
        if (windowWidth <= 0 || windowHeight <= 0) {
            glfwWaitEvents();
            continue;
        }

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        const float imageAspect =
            static_cast<float>(fb.getwidth()) / fb.getHeight();
        const float windowAspect =
            static_cast<float>(windowWidth) / windowHeight;
        float viewportWidth = 1.0f;
        float viewportHeight = 1.0f;
        if (windowAspect > imageAspect) {
            viewportWidth = imageAspect / windowAspect;
        } else {
            viewportHeight = windowAspect / imageAspect;
        }
        glViewport(
            static_cast<GLint>((1.0f - viewportWidth) * windowWidth / 2.0f),
            static_cast<GLint>((1.0f - viewportHeight) * windowHeight / 2.0f),
            static_cast<GLsizei>(viewportWidth * windowWidth),
            static_cast<GLsizei>(viewportHeight * windowHeight));

        glUseProgram(program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteTextures(1, &texture);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArray);
    glDeleteProgram(program);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}