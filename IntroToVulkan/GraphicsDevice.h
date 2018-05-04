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
	VkDevice m_virtualDevice;
	int m_graphicsFamilyQueue;

	void grabPhysicalDevice(GraphicsSystem& system);
	void findQueueFamilies();
	VkResult createVirtualDevice(GraphicsSystem& system);

public:
	GraphicsDevice();
	~GraphicsDevice() = default;

	VkResult initialize(GraphicsSystem& system);
	void cleanup();

	bool hasQueues() const { return m_graphicsFamilyQueue >= 0; }

	VkPhysicalDevice const& getDevice() const { return m_physicalDevice; }
};
