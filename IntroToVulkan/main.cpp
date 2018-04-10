#include <vulkan/vulkan.h>
#include <iostream>

#pragma comment(lib, "vulkan-1.lib")

int main()
{
	VkApplicationInfo app_info;
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = nullptr;
	app_info.pApplicationName = "My First Vulkan App";
	app_info.applicationVersion = 1;
	app_info.pEngineName = "Not-real Engine";
	app_info.engineVersion = 0;
	app_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instance_create_info;
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pNext = nullptr;
	instance_create_info.enabledLayerCount = 0;
	instance_create_info.enabledExtensionCount = 0;
	instance_create_info.flags = 0;
	instance_create_info.ppEnabledExtensionNames = nullptr;
	instance_create_info.ppEnabledLayerNames = nullptr;

	instance_create_info.pApplicationInfo = &app_info;

	VkInstance app_instance;

	VkResult success = vkCreateInstance(&instance_create_info, nullptr, &app_instance);
	if (success == VK_SUCCESS)
	{
		std::cout << "Success!" << std::endl;
	}
	else
	{
		std::cout << "Could not initialize Vulkan!" << std::endl;
		std::cout << "Error code: " << success << "." << std::endl;
	}

	std::cin.get();
	return 0;
}
