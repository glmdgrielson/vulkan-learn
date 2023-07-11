#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <cstdint>

class HelloTriangle {
public:
    void run() {
        this->initWindow();
        this->initVulkan();
        this->mainLoop();
        this->cleanup();
    }

private:
    GLFWwindow* window;
    const uint32_t HEIGHT = 800;
    const uint32_t WIDTH = 600;

    void initWindow() {
        // Initialize the graphics library?
        glfwInit();

        // Apparently we're not getting a window?
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        this->window = glfwCreateWindow(this->HEIGHT, this->WIDTH, "Vulkan", nullptr, nullptr);
    }

    VkInstance instance;

    void initVulkan() {
        //
    }

    void createInstance() {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_VERSION_1_0;
        appInfo.pNext = nullptr; // Leaving this empty for now.

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // We don't want extensions at the moment.
        uint32_t glfwExtensionC = 0;
        const char** glfwExtensionV = glfwGetRequiredInstanceExtensions(&glfwExtensionC);
        createInfo.enabledExtensionCount = glfwExtensionC;
        createInfo.ppEnabledExtensionNames = glfwExtensionV;

        createInfo.enabledLayerCount = 0; // no idea what this means.

        VkResult result = vkCreateInstance(&createInfo, nullptr, &this->instance);

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create instance.");
        }
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(this->window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        vkDestroyInstance(this->instance, nullptr);

        glfwDestroyWindow(this->window);

        glfwTerminate();
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
