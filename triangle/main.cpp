// Informative #includes. I hope.
#include <vulkan/vk_platform.h>
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
#include <cstdio>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
) {
    // Yes, I know C++ supposedly has a prettier version. I don't care,
    // I prefer this way. Fight me.
    switch (messageSeverity) {

    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
#ifndef NDEBUG
        fprintf(stderr, "Validation layer (verbose): %s\n", pCallbackData->pMessage);
#endif
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
#ifndef NDEBUG
        fprintf(stderr, "Validation layer (info): %s\n", pCallbackData->pMessage);
#endif
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        fprintf(stderr, "Validation layer (warning): %s\n", pCallbackData->pMessage);
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        fprintf(stderr, "Validation layer (error): %s\n", pCallbackData->pMessage);
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
      // A dummy value.
      break;
    }

    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger
) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
        instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr) {
      return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
      return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT messenger,
                                   const VkAllocationCallbacks *pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
        instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != nullptr) {
        func(instance, messenger, pAllocator);
    }
}

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
    VkDebugUtilsMessengerEXT messenger;

    void initVulkan() {
        this->createInstance();
        this->setupDebugMessenger();
    }

    void setupDebugUtilsMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
        createInfo.sType =
            VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
        createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    void setupDebugMessenger() {
        if (!this->enableValidationLayers) {
            return;
        }

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        this->setupDebugUtilsMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(this->instance, &createInfo, nullptr,
                                         &this->messenger) != VK_SUCCESS) {
            throw std::runtime_error("Failed to setup debug messenger!");
        }
    }

    std::vector<const char*> getRequiredExtensions() {
        uint32_t extensionsCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionsCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + extensionsCount);

        if (this->enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
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

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (this->enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(this->validationLayers.size());
            createInfo.ppEnabledLayerNames = this->validationLayers.data();

            this->setupDebugUtilsMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext =
                (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
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
        if (this->enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(this->instance, this->messenger, nullptr);
        }

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
