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
	VkQueue m_graphicsQueue, m_presentQueue;

	int m_graphicsFamilyQueue, m_presentationFamilyQueue;

	void grabPhysicalDevice(GraphicsSystem& system);
	void findQueueFamilies(GraphicsSystem& system);
	VkResult createVirtualDevice(GraphicsSystem& system);

public:
	GraphicsDevice();
	~GraphicsDevice() = default;

	VkResult initialize(GraphicsSystem& system);
	void cleanup();

	bool hasQueues() const { return m_graphicsFamilyQueue >= 0 && m_presentationFamilyQueue >= 0; }

	VkPhysicalDevice const& getDevice() const { return m_physicalDevice; }
};
