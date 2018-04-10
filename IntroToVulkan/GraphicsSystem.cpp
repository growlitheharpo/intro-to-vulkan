#include "GraphicsSystem.h"
#include <vector>
#include <iostream>

VkApplicationInfo GraphicsSystem::generateAppInfo()
{
	VkApplicationInfo app_info;
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = nullptr;
	app_info.pApplicationName = "My First Vulkan App";
	app_info.applicationVersion = 1;
	app_info.pEngineName = "Not-real Engine";
	app_info.engineVersion = 0;
	app_info.apiVersion = VK_API_VERSION_1_0;

	return app_info;
}

VkInstanceCreateInfo GraphicsSystem::generateCreateInfo()
{
	VkInstanceCreateInfo instance_create_info;
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pNext = nullptr;
	instance_create_info.enabledLayerCount = 0;
	instance_create_info.enabledExtensionCount = 0;
	instance_create_info.flags = 0;
	instance_create_info.ppEnabledExtensionNames = nullptr;
	instance_create_info.ppEnabledLayerNames = nullptr;
	instance_create_info.pApplicationInfo = nullptr;
	return instance_create_info;
}

VkResult GraphicsSystem::createInstance()
{
	VkApplicationInfo app_info = generateAppInfo();
	VkInstanceCreateInfo instance_create_info = generateCreateInfo();

	instance_create_info.pApplicationInfo = &app_info;
	return vkCreateInstance(&instance_create_info, nullptr, &m_vulkanInstance);
}

bool isDeviceSuitable(VkPhysicalDevice d)
{
	VkPhysicalDeviceProperties deviceProps;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(d, &deviceProps);
	vkGetPhysicalDeviceFeatures(d, &deviceFeatures);

	return deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 
		&& deviceFeatures.geometryShader;
}

VkResult GraphicsSystem::initializeDevice()
{
	uint32_t device_count = 0;
	vkEnumeratePhysicalDevices(m_vulkanInstance, &device_count, nullptr);

	if (device_count == 0)
		return VK_ERROR_INITIALIZATION_FAILED;

	std::vector<VkPhysicalDevice> devices(device_count);
	vkEnumeratePhysicalDevices(m_vulkanInstance, &device_count, devices.data());

	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			m_physicalDevice = device;
			break;
		}
	}

	if (device_count == 0 || m_physicalDevice == nullptr)
		return VK_ERROR_INITIALIZATION_FAILED;

	return VK_SUCCESS;
}

GraphicsSystem::GraphicsSystem() noexcept
	: m_vulkanInstance(nullptr), m_physicalDevice(nullptr)
{
}

VkResult GraphicsSystem::initialize()
{
	VkResult r = createInstance();
	if (r != VK_SUCCESS)
		return r;

	r = initializeDevice();
	return r;
}

void GraphicsSystem::cleanup()
{
	if (m_vulkanInstance == nullptr)
		return;

	vkDestroyInstance(m_vulkanInstance, nullptr);
}

void GraphicsSystem::displayExtensions()
{
	uint32_t extension_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
	std::vector<VkExtensionProperties> extensions(extension_count);

	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
	std::cout << "Available extensions: " << std::endl;
	for (const auto& e : extensions)
		std::cout << "\t" << e.extensionName << std::endl;
}
