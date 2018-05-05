#pragma once
#include <vulkan/vulkan.h>

class GraphicsSystem;

/**
 * \brief Class that represents a graphics device (i.e., a graphics card)
 */
class GraphicsDevice
{
private:
	struct QueueFamilyIndicies
	{
		int m_graphics, m_present;

		QueueFamilyIndicies() : m_graphics(-1), m_present(-1) { }

		void populate(VkPhysicalDevice device, GraphicsSystem& system);
		bool isComplete() const { return m_graphics >= 0 && m_present >= 0; }
		void reset() { m_graphics = -1; m_present = -1; }
	};

	VkPhysicalDevice m_physicalDevice;
	VkDevice m_virtualDevice;
	VkQueue m_graphicsQueue, m_presentQueue;

	QueueFamilyIndicies m_indicies;

	bool isDeviceSuitable(VkPhysicalDevice d, GraphicsSystem& system) const;
	void grabPhysicalDevice(GraphicsSystem& system);
	void findQueueFamilies(GraphicsSystem& system);
	VkResult createVirtualDevice(GraphicsSystem& system);

public:
	GraphicsDevice();
	~GraphicsDevice() = default;

	VkResult initialize(GraphicsSystem& system);
	void cleanup();

	bool hasQueues() const { return m_indicies.isComplete(); }

	VkPhysicalDevice const& getDevice() const { return m_physicalDevice; }
};
