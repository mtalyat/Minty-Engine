#include "pch.h"
#include "Minty/Rendering/M_Viewport.h"

#include "Minty/Math/M_Math.h"

using namespace Minty;

Minty::Viewport::Viewport()
	: _view({0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f})
	, _scissor({{0, 0}, {0u, 0u}})
{}

Minty::Viewport::Viewport(float const x, float const y, float const width, float const height, float const minDepth, float const maxDepth)
	: _view({x, y, width, height, minDepth, maxDepth})
	, _scissor({{Math::floor_to_int(x), Math::floor_to_int(y)}, {static_cast<unsigned int>(Math::floor_to_int(width)), static_cast<unsigned int>(Math::floor_to_int(height))}})
{}

Minty::Viewport::Viewport(int const x, int const y, unsigned int const width, unsigned int const height, float const minDepth, float const maxDepth)
	: _view({static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height), minDepth, maxDepth})
	, _scissor({{x, y}, {width, height}})
{}

void Minty::Viewport::bind(VkCommandBuffer const commandBuffer) const
{
	vkCmdSetViewport(commandBuffer, 0, 1, &_view);
	vkCmdSetScissor(commandBuffer, 0, 1, &_scissor);
}

void Minty::Viewport::set_offset(int const x, int const y)
{
	set_view_offset(x, y);
	set_scissor_offset(x, y);
}

void Minty::Viewport::set_view_offset(int const x, int const y)
{
	_view.x = static_cast<float>(x);
	_view.y = static_cast<float>(y);
}

void Minty::Viewport::set_scissor_offset(int const x, int const y)
{
	_scissor.offset = VkOffset2D
	{
		.x = x,
		.y = y
	};
}

void Minty::Viewport::set_extent(unsigned int width, unsigned int height)
{
	set_view_extent(width, height);
	set_scissor_extent(width, height);
}

void Minty::Viewport::set_view_extent(unsigned int const width, unsigned int const height)
{
	_view.width = static_cast<float>(width);
	_view.height = static_cast<float>(height);
}

void Minty::Viewport::set_scissor_extent(unsigned int const width, unsigned int const height)
{
	_scissor.extent = VkExtent2D{ 
		.width = width,
		.height = height 
	};
}