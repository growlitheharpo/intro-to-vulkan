#pragma once
#include <vulkan/vulkan.h>

class GraphicsWindow;
class GraphicsSystem;

class GraphicsSurface
{
private:
	VkSurfaceKHR m_surface;

public:
	GraphicsSurface();

	VkResult initialize(GraphicsSystem& gSystem, GraphicsWindow& window);
	void cleanup(GraphicsSystem& gSystem);
};
