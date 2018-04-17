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
		VkResult setupDebugCallback();

		VkDebugReportCallbackEXT m_debugCallback;
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugReportFlagsEXT flags,
			VkDebugReportObjectTypeEXT objType,
			uint64_t obj,
			size_t location,
			int32_t code,
			const char* layerPrefix,
			const char* msg,
			void* userData);
		
		static bool verifyExtensions();
		static bool verifyValidation();
		static VkApplicationInfo generateAppInfo();
		static VkInstanceCreateInfo generateCreateInfo();

		VkInstance getInstance() { return m_vulkanInstance; }
		VkInstance const& getInstance() const { return m_vulkanInstance; }

	public:
		GraphicsSystem() noexcept;

		VkResult initialize();
		void cleanup();
};
