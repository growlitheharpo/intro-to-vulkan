#pragma once
#include <vulkan/vulkan.h>

class GraphicsSystem;

/**
 * \brief Class that represents a graphics device (i.e., a graphics card)
 */
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
