#include "pch.h"
#include "M_Viewport.h"

minty::Viewport::Viewport()
	: _viewport({0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f})
	, _scissor({{0, 0}, {0u, 0u}})
{}

minty::Viewport::Viewport(int const x, int const y, unsigned int const width, unsigned int const height, float const minDepth, float const maxDepth)
	: _viewport({static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height), minDepth, maxDepth})
	, _scissor({{x, y}, {width, height}})
{}

void minty::Viewport::setOffset(int const x, int const y)
{
	setViewOffset(x, y);
	setScissorOffset(x, y);
}

void minty::Viewport::setViewOffset(int const x, int const y)
{
	_viewport.x = static_cast<float>(x);
	_viewport.y = static_cast<float>(y);
}

void minty::Viewport::setScissorOffset(int const x, int const y)
{
	_scissor.offset = VkOffset2D
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
	_viewport.width = static_cast<float>(width);
	_viewport.height = static_cast<float>(height);
}

void minty::Viewport::setScissorExtent(unsigned int const width, unsigned int const height)
{
	_scissor.extent = VkExtent2D{ 
		.width = width,
		.height = height 
	};
}
