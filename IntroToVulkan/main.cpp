#include <SDL.h>
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

	// SDL initialization (move to the graphics system later)
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* main_window = SDL_CreateWindow("Setup Window", 0, 0, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_DestroyWindow(main_window);
	SDL_Quit();
	
	return 0;
}
