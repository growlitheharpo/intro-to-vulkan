#pragma once
#include <vulkan/vulkan.h>

class GraphicsSystem
{
	private:
		VkInstance m_vulkanInstance;

		static VkApplicationInfo generateAppInfo();
		static VkInstanceCreateInfo generateCreateInfo();

	public:
		GraphicsSystem() noexcept;
		VkResult initialize();
};
