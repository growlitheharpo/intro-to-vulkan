#include <vulkan/vulkan.h>
#include <iostream>
#include "GraphicsSystem.h"

#pragma comment(lib, "vulkan-1.lib")

int main()
{
	GraphicsSystem graphics;
	const VkResult result = graphics.initialize();
	if (result == VK_SUCCESS)
	{
		std::cout << "Success!" << std::endl;
		graphics.displayExtensions();
	}
	else
	{
		std::cout << "Could not initialize Vulkan!" << std::endl;
		std::cout << "Error code: " << result << "." << std::endl;
	}

	std::cout << "Press Return to continue... ";
	std::cin.get();
	return 0;
}
