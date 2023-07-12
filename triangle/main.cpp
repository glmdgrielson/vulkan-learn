#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vk_enum_string_helper.h>

#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <cstring>

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

    const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

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
        this->createInstance();
        this->pickPhysicalDevice();
    }

    void createInstance() {
        if (this->enableValidationLayers && !this->checkValidationLayerSupport()) {
            throw std::runtime_error("Validation layers not loaded, oops.");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
        // appInfo.pNext = nullptr; // Leaving this empty for now.

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // We don't want extensions at the moment.
        uint32_t glfwExtensionC = 0;
        const char** glfwExtensionV = glfwGetRequiredInstanceExtensions(&glfwExtensionC);
        if (glfwExtensionV == nullptr) {
            throw std::runtime_error("Extensions are not loaded!");
        }
        createInfo.enabledExtensionCount = glfwExtensionC;
        createInfo.ppEnabledExtensionNames = glfwExtensionV;

        if (this->enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(this->validationLayers.size());
            createInfo.ppEnabledLayerNames = this->validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        VkResult result = vkCreateInstance(&createInfo, nullptr, &this->instance);

        if (result != VK_SUCCESS) {
            char err_msg[50];
            sprintf(err_msg, "Failed to create instance: %s", string_VkResult(result));
            throw std::runtime_error(err_msg);
        }
    }

    void pickPhysicalDevice() {
        VkPhysicalDevice device = VK_NULL_HANDLE;
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(this->instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            // No devices have been found. Uh oh.
            throw std::runtime_error("No suitable device found! Update your dang drivers!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(this->instance, &deviceCount, devices.data());

        for (const auto& maybeDevice : devices) {
            if (this->isDeviceSuitable(maybeDevice)) {
                device = maybeDevice;
                break;
            }
        }

        if (device == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to find physical device");
        }
    }

    bool isDeviceSuitable(VkPhysicalDevice device) {
        // Put in a dummy value just for now.
        return true;
    }

    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName: this->validationLayers) {
            bool layerFound = false;

            for (const auto &layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
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
