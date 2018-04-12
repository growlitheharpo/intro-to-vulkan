#pragma once
#include <vulkan/vulkan.h>

class GraphicsSystem;

class GraphicsDevice
{
private:
	VkPhysicalDevice m_physicalDevice;

public:
	GraphicsDevice();
	~GraphicsDevice();

	VkResult initialize(GraphicsSystem& system);
	void cleanup();

	VkPhysicalDevice const& getDevice() const { return m_physicalDevice; }
};
