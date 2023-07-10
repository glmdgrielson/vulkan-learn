#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <cstdio>
#include <stdexcept>

class HelloTriangle {
public:
    void run() {
        this->initVulkan();
        this->mainLoop();
        this->cleanup();
    }

private:
    void initVulkan() {
        //
    }

    void mainLoop() {
        //
    }

    void cleanup() {
        //
    }
};

int main() {
    HelloTriangle app;

    try {
        app.run();
    } catch (std::exception& err) {
        printf("%s\n", err.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
