#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <iostream>
#include "GraphicsSystem.h"
#include <string>

// *****************************************
//
// Vulkan is installed by following this link: 
// https://vulkan.lunarg.com/sdk/home#windows
//
// *****************************************

#pragma comment(lib, "vulkan-1.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2.lib")

int main(int , char *[])
{
	GraphicsSystem graphics;
	const VkResult result = graphics.initialize();

#ifndef NDEBUG
	if (result == VK_SUCCESS)
	{
		std::cout << "Initialize Vulkan: Success!" << std::endl;
	}
	else
	{
		std::cout << "Could not initialize Vulkan!" << std::endl;
		std::cout << "Error code: " << result << "." << std::endl;

		std::cout << "Press Return to continue...";
		std::cin.get();
		return result;
	}
#else
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to initialize Vulkan. Error code: " + std::to_string(result));
	}
#endif

	SDL_Event test;
	while (true)
	{
		SDL_PollEvent(&test);
		if (test.type == SDL_QUIT)
			break;
	}

	graphics.cleanup();

#ifndef NDEBUG
	std::cout << "Press Return to continue...";
	std::cin.get();
#endif

	return 0;
}
