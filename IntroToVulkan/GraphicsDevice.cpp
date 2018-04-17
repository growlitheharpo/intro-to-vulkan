#include "GraphicsDevice.h"
#include <vector>
#include "GraphicsSystem.h"


GraphicsDevice::GraphicsDevice() : m_physicalDevice(nullptr)
{
}


GraphicsDevice::~GraphicsDevice()
{
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

VkResult GraphicsDevice::initialize(GraphicsSystem& system)
{
	VkInstance vulkan_instance = system.getInstance();

	uint32_t device_count = 0;
	vkEnumeratePhysicalDevices(vulkan_instance, &device_count, nullptr);

	if (device_count == 0)
		return VK_ERROR_INITIALIZATION_FAILED;

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

	if (device_count == 0 || m_physicalDevice == nullptr)
		return VK_ERROR_INITIALIZATION_FAILED;

	return VK_SUCCESS;
}

void GraphicsDevice::cleanup()
{
}
