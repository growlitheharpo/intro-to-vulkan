#pragma once
#include <vulkan/vulkan.h>

class GraphicsSystem
{
	private:
		VkInstance m_vulkanInstance;
		VkPhysicalDevice m_physicalDevice;

		static VkApplicationInfo generateAppInfo();
		static VkInstanceCreateInfo generateCreateInfo();

		VkResult createInstance();
		VkResult initializeDevice();

	public:
		GraphicsSystem() noexcept;

		VkResult initialize();
		void cleanup();

		void displayExtensions();
};
