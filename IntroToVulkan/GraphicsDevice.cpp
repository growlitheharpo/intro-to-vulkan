#include "GraphicsDevice.h"
#include <vector>
#include "GraphicsSystem.h"
#include <set>

bool GraphicsDevice::isDeviceSuitable(VkPhysicalDevice d, GraphicsSystem& system) const
{
	VkPhysicalDeviceProperties deviceProps;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(d, &deviceProps);
	vkGetPhysicalDeviceFeatures(d, &deviceFeatures);

	QueueFamilyIndicies indicies;
	indicies.populate(d, system);

	return deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
		&& deviceFeatures.geometryShader
		&& indicies.isComplete();
}

void GraphicsDevice::QueueFamilyIndicies::populate(VkPhysicalDevice device, GraphicsSystem& system)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	const VkSurfaceKHR draw_surface = system.m_drawSurface.m_surface;

	for (size_t i = 0; i < queueFamilies.size(); ++i)
	{
		if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			m_graphics = i;
		}

		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, draw_surface, &presentationSupport);
		if (queueFamilies[i].queueCount > 0 && presentationSupport)
		{
			m_present = i;
		}

		if (isComplete())
			break;
	}
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
		if (isDeviceSuitable(device, system))
		{
			m_physicalDevice = device;
			break;
		}
	}
}

void GraphicsDevice::findQueueFamilies(GraphicsSystem& system)
{
	m_indicies.populate(m_physicalDevice, system);
}

VkResult GraphicsDevice::createVirtualDevice(GraphicsSystem& system)
{
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	VkPhysicalDeviceFeatures deviceFeatures = {};
	VkDeviceCreateInfo createInfo = {};
	float queuePriority = 1.0f;

	// Fill the queue create info
	std::set<int> uniqueQueueFamilies = { m_indicies.m_graphics, m_indicies.m_present };
	for (int queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	// TODO: Fill in the device features struct

	// Fill the device create info
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
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

GraphicsDevice::GraphicsDevice() :
	m_physicalDevice(nullptr),
	m_virtualDevice(nullptr),
	m_graphicsQueue(nullptr),
	m_presentQueue(nullptr)
{
}

VkResult GraphicsDevice::initialize(GraphicsSystem& system)
{
	grabPhysicalDevice(system);
	if (m_physicalDevice == nullptr)
		return VK_ERROR_INITIALIZATION_FAILED;

	findQueueFamilies(system);
	if (!hasQueues())
		return VK_ERROR_INITIALIZATION_FAILED;

	const VkResult r = createVirtualDevice(system);
	if (r != VK_SUCCESS)
		return r;

	vkGetDeviceQueue(m_virtualDevice, m_indicies.m_graphics, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_virtualDevice, m_indicies.m_present, 0, &m_presentQueue);
	return VK_SUCCESS;
}

void GraphicsDevice::cleanup()
{
	vkDestroyDevice(m_virtualDevice, nullptr);
	m_virtualDevice = nullptr;
	m_graphicsQueue = nullptr;


	m_physicalDevice = nullptr;
	m_indicies.reset();
}
