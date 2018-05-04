#include "GraphicsDevice.h"
#include <vector>
#include "GraphicsSystem.h"

bool isDeviceSuitable(VkPhysicalDevice d)
{
	VkPhysicalDeviceProperties deviceProps;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(d, &deviceProps);
	vkGetPhysicalDeviceFeatures(d, &deviceFeatures);

	return deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
		&& deviceFeatures.geometryShader;
}

void GraphicsDevice::grabPhysicalDevice(GraphicsSystem& system)
{
	VkInstance vulkan_instance = system.getInstance();

	uint32_t device_count = 0;
	vkEnumeratePhysicalDevices(vulkan_instance, &device_count, nullptr);

	if (device_count == 0)
		return;

	std::vector<VkPhysicalDevice> devices(device_count);
	vkEnumeratePhysicalDevices(vulkan_instance, &device_count, devices.data());

	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			m_physicalDevice = device;
			break;
		}
	}
}

void GraphicsDevice::findQueueFamilies()
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

	for (size_t i = 0; i < queueFamilies.size(); ++i)
	{
		if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			m_graphicsFamilyQueue = i;
			break;
		}
	}
}

VkResult GraphicsDevice::createVirtualDevice(GraphicsSystem& system)
{
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	VkPhysicalDeviceFeatures deviceFeatures = {};
	VkDeviceCreateInfo createInfo = {};
	float queuePriority = 1.0f;

	// Fill the queue create info
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = m_graphicsFamilyQueue;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	// TODO: Fill in the device features struct

	// Fill the device create info
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = 0;
	if (GraphicsSystem::ENABLE_VALIDATION)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(GraphicsSystem::required_validation.size());
		createInfo.ppEnabledLayerNames = GraphicsSystem::required_validation.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	return vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_virtualDevice);
}

GraphicsDevice::GraphicsDevice() : m_physicalDevice(nullptr), m_virtualDevice(nullptr), m_graphicsFamilyQueue(-1)
{
}

VkResult GraphicsDevice::initialize(GraphicsSystem& system)
{
	grabPhysicalDevice(system);
	if (m_physicalDevice == nullptr)
		return VK_ERROR_INITIALIZATION_FAILED;

	findQueueFamilies();
	if (!hasQueues())
		return VK_ERROR_INITIALIZATION_FAILED;

	return createVirtualDevice(system);
}

void GraphicsDevice::cleanup()
{
	vkDestroyDevice(m_virtualDevice, nullptr);

	m_physicalDevice = nullptr;
	m_graphicsFamilyQueue = -1;
}
