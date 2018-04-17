#include "GraphicsSystem.h"
#include <vector>
#include <iostream>
#include <array>

std::array<const char*, 2> required_extensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };

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
	instance_create_info.enabledLayerCount = 0;
	instance_create_info.enabledExtensionCount = static_cast<uint32_t>(required_extensions.size());
	instance_create_info.flags = 0;
	instance_create_info.ppEnabledExtensionNames = required_extensions.data();
	instance_create_info.ppEnabledLayerNames = nullptr;
	instance_create_info.pApplicationInfo = nullptr;
	return instance_create_info;
}

VkResult GraphicsSystem::createInstance()
{
	if (!verifyExtensions())
		return VK_ERROR_INITIALIZATION_FAILED;

	VkApplicationInfo app_info = generateAppInfo();
	VkInstanceCreateInfo instance_create_info = generateCreateInfo();

	instance_create_info.pApplicationInfo = &app_info;
	return vkCreateInstance(&instance_create_info, nullptr, &m_vulkanInstance);
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

	r = m_graphicsDevice.initialize(*this);
	if (r != VK_SUCCESS)
		return r;

	r = m_graphicsWindow.initialize();
	if (r != VK_SUCCESS)
		return r;

	r = m_drawSurface.initialize(*this, m_graphicsWindow);
	if (r != VK_SUCCESS)
		return r;

	return r;
}

void GraphicsSystem::cleanup()
{
	if (m_vulkanInstance == nullptr)
		return;

	m_graphicsWindow.cleanup();
	m_graphicsDevice.cleanup();
	vkDestroyInstance(m_vulkanInstance, nullptr);
}

bool GraphicsSystem::verifyExtensions() const
{
	uint32_t extension_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties> extensions(extension_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

	uint8_t found = 0;

	std::cout << "Available extensions: " << std::endl;
	for (uint32_t i = 0; i < extension_count; ++i)
	{
		std::cout << "\t" << extensions[i].extensionName << std::endl;
		
		for (auto const& required_extension : required_extensions)
		{
			if (strcmp(required_extension, extensions[i].extensionName) == 0)
				++found;
		}
	}

	if (found == required_extensions.size())
	{
		std::cout << "All required extensions found." << std::endl;
		return true;
	}
	else
	{
		std::cout << "WARNING: Could not locate all extensions." << std::endl;
		return false;
	}
}
