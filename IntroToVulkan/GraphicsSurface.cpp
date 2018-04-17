#include "GraphicsSurface.h"
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#include "GraphicsWindow.h"
#include "GraphicsSystem.h"

GraphicsSurface::GraphicsSurface() : m_surface(nullptr)
{
}

VkResult GraphicsSurface::initialize(GraphicsSystem& gSystem, GraphicsWindow& window)
{
	VkWin32SurfaceCreateInfoKHR surfaceInfo;
	surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.pNext = nullptr;
	surfaceInfo.flags = 0;
	surfaceInfo.hinstance = GetModuleHandle(nullptr);
	surfaceInfo.hwnd = window.getWindowHandle();

	return vkCreateWin32SurfaceKHR(gSystem.getInstance(), &surfaceInfo, nullptr, &m_surface);
}

void GraphicsSurface::cleanup(GraphicsSystem& gSystem)
{
	vkDestroySurfaceKHR(gSystem.getInstance(), m_surface, nullptr);
	m_surface = nullptr;
}
