#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <iostream>
#include "GraphicsSystem.h"

#pragma comment(lib, "vulkan-1.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2.lib")

int main(int argc, char *args[])
{
	GraphicsSystem graphics;
	const VkResult result = graphics.initialize();
	if (result == VK_SUCCESS)
	{
		std::cout << "Success!" << std::endl;
	}
	else
	{
		std::cout << "Could not initialize Vulkan!" << std::endl;
		std::cout << "Error code: " << result << "." << std::endl;
	}

	return 0;
}
