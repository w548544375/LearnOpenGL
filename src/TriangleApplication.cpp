#include "TriangleApplication.h"

const int WIDTH = 800;
const int HEIGHT= 600;
#if defined(__linux__) || defined(__linux)
const std::vector<const char*> validationLayers = {
  "VK_LAYER_LUNARG_standard_validation"  
};
#else
const std::vector<const char*> validationLayers = {
  "VK_LAYER_KHRONOS_validation"  
};
#endif


const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
    const bool enableValidataionLayers = false;
#else
    const bool enableValidataionLayers = true;
#endif


VkResult TriangleApplication::CreateDebugUtilsMessengerEXT(VkInstance instance,
                        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
                        const VkAllocationCallbacks* pAllocator, 
                        VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}


void TriangleApplication::DestroyDebugUtilsMessengerEXT(VkInstance instance,
        VkDebugUtilsMessengerEXT callback,
        const VkAllocationCallbacks * pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(instance,"vkDestroyDebugUtilsMessengerEXT"));
    if(func != nullptr){
        func(instance,callback,pAllocator);
    }
}


void TriangleApplication::run()
{
    this->initWindow();
    this->initVulkan();
    this->mainLoop();
    this->cleanup();
}

void TriangleApplication::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);
    this->window = glfwCreateWindow(WIDTH,HEIGHT,"Vulkan",nullptr,nullptr);

}

void TriangleApplication::initVulkan()
{
    this->createInstance();
    this->setupDebugMessenger();
    this->createSurface();
    this->pickPhysicalDevice();
    this->createLogicalDevice();
}



void TriangleApplication::createInstance()
{
    if(enableValidataionLayers && !checkValidationLayerSupport())
    {
        throw std::runtime_error("validation layers requestd,but not available!");
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Trangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    //std::cout << "GLFW扩展:" << glfwExtensionCount << std::endl;

    // uint32_t glfwExtensionCount = 0;
    // const char** glfwExtensions;
    // glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    // std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    // if (enableValidataionLayers) {
    //         extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    // }
    auto extensions = getRequiredExtensions();

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if(enableValidataionLayers){
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }else{
        createInfo.enabledLayerCount = 0;
    }
    //uint32_t extensoinCount = 0;
    // vkEnumerateInstanceExtensionProperties(nullptr,&extensoinCount,nullptr);

    // std::vector<VkExtensionProperties> extensions(extensoinCount) ;
    // vkEnumerateInstanceExtensionProperties(nullptr, &extensoinCount, extensions.data());
    // std::cout << "可用的扩展["<< extensoinCount << "]:" << std::endl; 
    // for(const auto& extension : extensions)
    // {
    //     std::cout << "\t" << extension.extensionName << std::endl;
    // }
    // std::cout << "打印完成！" << std::endl;
    VkResult result = vkCreateInstance(&createInfo,nullptr,&instance);
    if (result != VK_SUCCESS){
        throw std::runtime_error("failed to create vkinstance!");
    }
}

void TriangleApplication::setupDebugMessenger()
{
    if(!enableValidataionLayers) return;
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = 
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType =
                       VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;
    if(CreateDebugUtilsMessengerEXT(instance,&createInfo,nullptr,&debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug callback!");
    }
}


bool TriangleApplication::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceExtensionProperties(nullptr,&layerCount,nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount,availableLayers.data());
    for(const char * layerName : validationLayers)
    {
        bool layerFound = false;
        for(const auto& layerProperty : availableLayers)
        {
            std::cout << layerProperty.layerName << std::endl;
            if(strcmp(layerName,layerProperty.layerName) == 0){
                layerFound = true;
                break;
            }
        }
        if(!layerFound){
                return false;
            }
        return true;
    }
    return false;
}

std::vector<const char *> TriangleApplication::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidataionLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL TriangleApplication::debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT mesageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void * pUserData)
{
    std::cerr << "validation layer:" << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}


void TriangleApplication::mainLoop()
{
        while (!glfwWindowShouldClose(this->window))
        {
            glfwPollEvents();
        }
        
}

void TriangleApplication::cleanup()
{
        vkDestroyDevice(device,nullptr);
        if(enableValidataionLayers)
        {
            DestroyDebugUtilsMessengerEXT(instance,debugMessenger,nullptr);
        }
        vkDestroySurfaceKHR(instance,surface,nullptr);
        vkDestroyInstance(instance,nullptr);
        glfwDestroyWindow(this->window);
        glfwTerminate();
}


void TriangleApplication::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance,&deviceCount,nullptr);
    if(deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance,&deviceCount,devices.data());
    for(const auto& device : devices)
    {
        if(isDeviceSuitable(device))
        {
            physicalDevice = device;
            break;
        }
    }
    if(physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }


}


bool TriangleApplication::isDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = findQueueFamilies(device);
    bool extensionSupported =  this->checkDeviceExtensionSupport(device);
    bool swapChainAdequate = false;
    if(extensionSupported)
    {
        SwapChainSupportDetails SwapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !SwapChainSupport.formats.empty() && !SwapChainSupport.presentModes.empty();
    }
    return indices.isComplete() && extensionSupported && swapChainAdequate;
}

bool TriangleApplication::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device,nullptr,&extensionCount,nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device,nullptr,&extensionCount,availableExtensions.data());
    std::set<std::string> requiredExtensions(deviceExtensions.begin(),deviceExtensions.end());
    for(const auto&extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }
    return requiredExtensions.empty();
}

// 队列族
QueueFamilyIndices TriangleApplication::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount) ;
    vkGetPhysicalDeviceQueueFamilyProperties (device, &queueFamilyCount ,queueFamilies.data());
    int i = 0;
    for(const auto& queueFamily : queueFamilies) {
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device,i,surface,&presentSupport);
        if(presentSupport)
        {
            indices.presentFamily = i;
        }
        if(indices.isComplete()) {
            break;
        }
        i++;
    }
    return indices;
}

// 逻辑设备
void TriangleApplication::createLogicalDevice()
{
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
       std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};
        float queuePriority = 1.0f;
        for(uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        // 指定使用设备特性
        VkPhysicalDeviceFeatures deviceFeatures = {};

        // 创建逻辑设备
        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if(enableValidataionLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }else {
            createInfo.enabledLayerCount = 0;
        }

        if(vkCreateDevice(physicalDevice,&createInfo,nullptr,&device) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }
        vkGetDeviceQueue(device,indices.graphicsFamily.value(),0,&graphicsQueue);
        vkGetDeviceQueue(device,indices.presentFamily.value(),0,&presentQueue);
}

void TriangleApplication::createSurface()
{
    if(glfwCreateWindowSurface(instance,window,nullptr,&surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
}

SwapChainSupportDetails TriangleApplication::querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }
    return details;
}