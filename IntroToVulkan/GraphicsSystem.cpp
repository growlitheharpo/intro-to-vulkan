#include "GraphicsSystem.h"
#include <vector>
#include <iostream>

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
	instance_create_info.enabledExtensionCount = 0;
	instance_create_info.flags = 0;
	instance_create_info.ppEnabledExtensionNames = nullptr;
	instance_create_info.ppEnabledLayerNames = nullptr;
	instance_create_info.pApplicationInfo = nullptr;
	return instance_create_info;
}

GraphicsSystem::GraphicsSystem() noexcept
	: m_vulkanInstance(nullptr)
{
}

VkResult GraphicsSystem::initialize()
{
	VkApplicationInfo app_info = generateAppInfo();
	VkInstanceCreateInfo instance_create_info = generateCreateInfo();

	instance_create_info.pApplicationInfo = &app_info;

	return vkCreateInstance(&instance_create_info, nullptr, &m_vulkanInstance);
}

void GraphicsSystem::cleanup()
{
	if (m_vulkanInstance == nullptr)
		return;

	vkDestroyInstance(m_vulkanInstance, nullptr);
}

void GraphicsSystem::displayExtensions()
{
	uint32_t extension_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
	std::vector<VkExtensionProperties> extensions(extension_count);

	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
	std::cout << "Available extensions: " << std::endl;
	for (const auto& e : extensions)
		std::cout << "\t" << e.extensionName << std::endl;
}
