#pragma once
#include <vulkan/vulkan.h>
#include "GraphicsDevice.h"
#include "GraphicsWindow.h"
#include "GraphicsSurface.h"

class GraphicsSystem
{
	friend class GraphicsDevice;
	friend class GraphicsSurface;

	private:
		VkInstance m_vulkanInstance;
		GraphicsDevice m_graphicsDevice;
		GraphicsWindow m_graphicsWindow;
		GraphicsSurface m_drawSurface;

		VkResult createInstance();
		bool verifyExtensions() const;

		static VkApplicationInfo generateAppInfo();
		static VkInstanceCreateInfo generateCreateInfo();

		VkInstance getInstance() { return m_vulkanInstance; }
		VkInstance const& getInstance() const { return m_vulkanInstance; }

	public:
		GraphicsSystem() noexcept;

		VkResult initialize();
		void cleanup();
};
