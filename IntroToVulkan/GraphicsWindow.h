#pragma once
#include <SDL.h>
#include <Windows.h>
#include <vulkan/vulkan.h>

class GraphicsWindow
{
	friend class GraphicsSurface;

private:
	SDL_Window * m_window;

	HWND getWindowHandle() const;
public:
	GraphicsWindow();
	virtual ~GraphicsWindow();

	VkResult initialize();
	void cleanup();
};
