#include "GraphicsSystem.h"
#include <vector>
#include <array>
#include "DEBUG_LOG.h"

VkApplicationInfo GraphicsSystem::generateAppInfo()
{
	VkApplicationInfo app_info;
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = nullptr;
	app_info.pApplicationName = "My First Vulkan App";
	app_info.applicationVersion = 1;
	app_info.pEngineName = "Not-real Engine";
	app_info.engineVersion = 0;
	app_info.apiVersion = VK_API_VERSION_1_0;

	return app_info;
}

VkInstanceCreateInfo GraphicsSystem::generateCreateInfo()
{
	VkInstanceCreateInfo instance_create_info;
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pNext = nullptr;
	instance_create_info.flags = 0;
	instance_create_info.pApplicationInfo = nullptr;

	if (ENABLE_VALIDATION)
	{
		instance_create_info.enabledLayerCount = static_cast<uint32_t>(required_validation.size());
		instance_create_info.ppEnabledLayerNames = required_validation.data();
	}
	else 
	{
		instance_create_info.enabledLayerCount = 0;
		instance_create_info.ppEnabledLayerNames = nullptr;
	}

	instance_create_info.enabledExtensionCount = static_cast<uint32_t>(g_required_extensions.size());
	instance_create_info.ppEnabledExtensionNames = g_required_extensions.data();
	return instance_create_info;
}

VkResult GraphicsSystem::createInstance()
{
	if (!verifyExtensions())
		return VK_ERROR_INITIALIZATION_FAILED;

	if (ENABLE_VALIDATION && !verifyValidation())
		return VK_ERROR_INITIALIZATION_FAILED;

	VkApplicationInfo app_info = generateAppInfo();
	VkInstanceCreateInfo instance_create_info = generateCreateInfo();

	instance_create_info.pApplicationInfo = &app_info;
	return vkCreateInstance(&instance_create_info, nullptr, &m_vulkanInstance);
}

VkResult GraphicsSystem::setupDebugCallback()
{
	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = debugCallback;
	
	// ReSharper disable once CppLocalVariableMayBeConst
	auto func = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(m_vulkanInstance, "vkCreateDebugReportCallbackEXT"));

	if (func == nullptr)
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	
	return func(m_vulkanInstance, &createInfo, nullptr, m_debugCallback);
}


VkBool32 GraphicsSystem::debugCallback(VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT, uint64_t, size_t, int32_t, const char*, const char* msg, void*)
{
	DEBUG_LOG_CONTINUOUS("Validation Layer Says: ");
	DEBUG_LOG(msg);

	return VK_FALSE;
}

GraphicsSystem::GraphicsSystem() noexcept
	: m_vulkanInstance(nullptr)
{
}

VkResult GraphicsSystem::initialize()
{
	VkResult r = createInstance();
	if (r != VK_SUCCESS)
		return r;

	if (ENABLE_VALIDATION)
	{
		r = setupDebugCallback();
		if (r != VK_SUCCESS)
			return r;
	}

	r = m_graphicsWindow.initialize();
	if (r != VK_SUCCESS)
		return r;

	r = m_drawSurface.initialize(*this, m_graphicsWindow);
	if (r != VK_SUCCESS)
		return r;

	r = m_graphicsDevice.initialize(*this);
	if (r != VK_SUCCESS)
		return r;

	return r;
}

void GraphicsSystem::cleanup()
{
	if (m_vulkanInstance == nullptr)
		return;

	m_drawSurface.cleanup(*this);
	m_graphicsWindow.cleanup();
	m_graphicsDevice.cleanup();

	if (ENABLE_VALIDATION)
		cleanupDebugCallback();

	vkDestroyInstance(m_vulkanInstance, nullptr);
}

void GraphicsSystem::cleanupDebugCallback()
{
	const auto func = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(m_vulkanInstance, "vkDestroyDebugReportCallbackEXT"));
	if (func != nullptr)
		func(m_vulkanInstance, *m_debugCallback, nullptr);
}

bool GraphicsSystem::verifyExtensions()
{
	uint32_t extension_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties> extensions(extension_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

	uint8_t found = 0;

	DEBUG_LOG("Available extensions: ");
	for (uint32_t i = 0; i < extension_count; ++i)
	{
		DEBUG_LOG_CONTINUOUS("\t");
		DEBUG_LOG_CONTINUOUS(extensions[i].extensionName);
		DEBUG_LOG_CONTINUOUS("\n");
		
		for (auto const& required_extension : g_required_extensions)
		{
			if (strcmp(required_extension, extensions[i].extensionName) == 0)
				++found;
		}
	}

	if (found == g_required_extensions.size())
	{
		DEBUG_LOG("All required extensions found.");
		return true;
	}
	else
	{
		DEBUG_LOG("WARNING: Could not locate all extensions.");
		return false;
	}
}

bool GraphicsSystem::verifyValidation()
{
	uint32_t count = 0;
	vkEnumerateInstanceLayerProperties(&count, nullptr);

	std::vector<VkLayerProperties> availableLayers(count);
	vkEnumerateInstanceLayerProperties(&count, availableLayers.data());

	uint8_t found = 0;
	for (uint32_t i = 0; i < count; ++i)
	{
		for (auto const& layer : required_validation)
		{
			if (strcmp(layer, availableLayers[i].layerName) == 0)
				++found;
		}
	}

	if (found == required_validation.size())
	{
		DEBUG_LOG("All required validation layers found.");
		return true;
	}
	else
	{
		DEBUG_LOG("WARNING: Could not locate all validation layers.");
		return false;
	}
}
