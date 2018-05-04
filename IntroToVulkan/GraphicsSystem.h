#pragma once
#include <vulkan/vulkan.h>
#include "GraphicsDevice.h"
#include "GraphicsWindow.h"
#include "GraphicsSurface.h"
#include <array>

class GraphicsSystem
{
	friend class GraphicsDevice;
	friend class GraphicsSurface;

	static constexpr std::array<const char*, 1> required_validation = { "VK_LAYER_LUNARG_standard_validation" };

#if NDEBUG
	static const bool ENABLE_VALIDATION = false;
	static constexpr std::array<const char*, 2> required_extensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };
#else
	static const bool ENABLE_VALIDATION = true;
	static constexpr std::array<const char*, 3> g_required_extensions = { "VK_KHR_surface", "VK_KHR_win32_surface", VK_EXT_DEBUG_REPORT_EXTENSION_NAME };
#endif

	private:
		VkInstance m_vulkanInstance;
		GraphicsDevice m_graphicsDevice;
		GraphicsWindow m_graphicsWindow;
		GraphicsSurface m_drawSurface;

		VkResult createInstance();
		VkResult setupDebugCallback();

		void cleanupDebugCallback();

		VkDebugReportCallbackEXT m_debugCallback[1];
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugReportFlagsEXT flags,
			VkDebugReportObjectTypeEXT obj_type,
			uint64_t obj,
			size_t location,
			int32_t code,
			const char* layer_prefix,
			const char* msg,
			void* user_data);

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
