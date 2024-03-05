#include "pch.h"
#include "M_Viewport.h"

#include "M_Math.h"

using namespace minty;

minty::Viewport::Viewport()
	: _view({0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f})
	, _scissor({{0, 0}, {0u, 0u}})
{}

minty::Viewport::Viewport(float const x, float const y, float const width, float const height, float const minDepth, float const maxDepth)
	: _view({x, y, width, height, minDepth, maxDepth})
	, _scissor({{Math::floor_to_int(x), Math::floor_to_int(y)}, {static_cast<unsigned int>(Math::floor_to_int(width)), static_cast<unsigned int>(Math::floor_to_int(height))}})
{}

minty::Viewport::Viewport(int const x, int const y, unsigned int const width, unsigned int const height, float const minDepth, float const maxDepth)
	: _view({static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height), minDepth, maxDepth})
	, _scissor({{x, y}, {width, height}})
{}

void minty::Viewport::bind(VkCommandBuffer const commandBuffer) const
{
	vkCmdSetViewport(commandBuffer, 0, 1, &_view);
	vkCmdSetScissor(commandBuffer, 0, 1, &_scissor);
}

void minty::Viewport::set_offset(int const x, int const y)
{
	set_view_offset(x, y);
	set_scissor_offset(x, y);
}

void minty::Viewport::set_view_offset(int const x, int const y)
{
	_view.x = static_cast<float>(x);
	_view.y = static_cast<float>(y);
}

void minty::Viewport::set_scissor_offset(int const x, int const y)
{
	_scissor.offset = VkOffset2D
	{
		.x = x,
		.y = y
	};
}

void minty::Viewport::set_extent(unsigned int width, unsigned int height)
{
	set_view_extent(width, height);
	set_scissor_extent(width, height);
}

void minty::Viewport::set_view_extent(unsigned int const width, unsigned int const height)
{
	_view.width = static_cast<float>(width);
	_view.height = static_cast<float>(height);
}

void minty::Viewport::set_scissor_extent(unsigned int const width, unsigned int const height)
{
	_scissor.extent = VkExtent2D{ 
		.width = width,
		.height = height 
	};
}

VkViewport minty::Viewport::get_view() const
{
	return _view;
}

VkRect2D minty::Viewport::get_scissor() const
{
	return _scissor;
}

String minty::to_string(Viewport const& value)
{
	return std::format("Viewport(View(x = {}, y = {}, width = {}, height = {}, min depth = {}, max depth = {}), Scissor(x = {}, y = {}, width = {}, height = {}))", value._view.x, value._view.y, value._view.width, value._view.height, value._view.minDepth, value._view.maxDepth, value._scissor.offset.x, value._scissor.offset.y, value._scissor.extent.width, value._scissor.extent.height);
}
