#include "GraphicsWindow.h"
#include "SDL_syswm.h"

GraphicsWindow::GraphicsWindow()
{
	m_window = nullptr;
}

GraphicsWindow::~GraphicsWindow()
{
	cleanup();
}

VkResult GraphicsWindow::initialize()
{
	SDL_Init(SDL_INIT_VIDEO);
	m_window = SDL_CreateWindow("Setup Window", 0, 0, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (m_window == nullptr)
		return VK_ERROR_INITIALIZATION_FAILED;

	return VK_SUCCESS;
}

void GraphicsWindow::cleanup()
{
	if (m_window == nullptr)
		return;

	SDL_DestroyWindow(m_window);
	m_window = nullptr;
	SDL_Quit();
}

HWND GraphicsWindow::getWindowHandle() const
{
	if (m_window == nullptr)
		return nullptr;

	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);

	if (SDL_GetWindowWMInfo(m_window, &info))
	{
		return info.info.win.window;
	}

	return nullptr;
}
