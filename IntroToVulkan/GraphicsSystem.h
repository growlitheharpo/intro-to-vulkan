#pragma once
#include <vulkan/vulkan.h>
#include "GraphicsDevice.h"

class GraphicsSystem
{
	friend class GraphicsDevice;

	private:
		VkInstance m_vulkanInstance;

		GraphicsDevice m_graphicsDevice;

		static VkApplicationInfo generateAppInfo();
		static VkInstanceCreateInfo generateCreateInfo();

		VkResult createInstance();

		bool verifyExtensions() const;

		VkInstance getInstance() { return m_vulkanInstance; }
		VkInstance const& getInstance() const { return m_vulkanInstance; }

	public:
		GraphicsSystem() noexcept;

		VkResult initialize();
		void cleanup();
};
