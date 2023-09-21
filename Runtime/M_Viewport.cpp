#include "pch.h"
#include "M_Viewport.h"

minty::Viewport::Viewport()
	: view({0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f})
	, scissor({{0, 0}, {0u, 0u}})
{}

minty::Viewport::Viewport(int const x, int const y, unsigned int const width, unsigned int const height, float const minDepth, float const maxDepth)
	: view({static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height), minDepth, maxDepth})
	, scissor({{x, y}, {width, height}})
{}

void minty::Viewport::setOffset(int const x, int const y)
{
	setViewOffset(x, y);
	setScissorOffset(x, y);
}

void minty::Viewport::setViewOffset(int const x, int const y)
{
	view.x = static_cast<float>(x);
	view.y = static_cast<float>(y);
}

void minty::Viewport::setScissorOffset(int const x, int const y)
{
	scissor.offset = VkOffset2D
	{
		.x = x,
		.y = y
	};
}

void minty::Viewport::setExtent(unsigned int width, unsigned int height)
{
	setViewExtent(width, height);
	setScissorExtent(width, height);
}

void minty::Viewport::setViewExtent(unsigned int const width, unsigned int const height)
{
	view.width = static_cast<float>(width);
	view.height = static_cast<float>(height);
}

void minty::Viewport::setScissorExtent(unsigned int const width, unsigned int const height)
{
	scissor.extent = VkExtent2D{ 
		.width = width,
		.height = height 
	};
}
