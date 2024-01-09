#include "pch.h"
#include "M_Rendering_DescriptorSet.h"

#include "M_RenderEngine.h"

using namespace minty;
using namespace minty::rendering;

minty::rendering::DescriptorSet::DescriptorSet(RenderEngine& renderer)
	: RenderObject::RenderObject(renderer)
	, _descriptorSets()
{}

minty::rendering::DescriptorSet::DescriptorSet(std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> const& descriptorSets, std::unordered_map<String, std::array<ID, MAX_FRAMES_IN_FLIGHT>> const& buffers, RenderEngine& renderer)
	: RenderObject::RenderObject(renderer)
	, _descriptorSets(descriptorSets)
	, _buffers(buffers)
{}

void minty::rendering::DescriptorSet::operator=(DescriptorSet const& other)
{
	_descriptorSets = other._descriptorSets;
	_buffers = other._buffers;
}

void minty::rendering::DescriptorSet::destroy()
{
	for (size_t i = 0; i < _descriptorSets.size(); i++)
	{
		_descriptorSets[i] = VK_NULL_HANDLE;
	}
	_buffers.clear();
}

void minty::rendering::DescriptorSet::set(String const& name, void* const value) const
{
	// get buffer ids
	auto const& bufferIds = _buffers.at(name);

	// set all buffers
	for (auto const id : bufferIds)
	{
		_renderer.set_buffer(id, value);
	}
}

void minty::rendering::DescriptorSet::set(String const& name, int const frame, void* const value) const
{
	// get buffer id
	ID bufferId = _buffers.at(name).at(frame);

	// set buffer
	_renderer.set_buffer(bufferId, value);
}

void minty::rendering::DescriptorSet::set(String const& name, void* const value, VkDeviceSize const size, VkDeviceSize const offset) const
{
	// get buffer ids
	auto const& bufferIds = _buffers.at(name);

	// set all buffers
	for (auto const id : bufferIds)
	{
		_renderer.set_buffer(id, value, offset, size);
	}
}

void minty::rendering::DescriptorSet::set(String const& name, int const frame, void* const value, VkDeviceSize const size, VkDeviceSize const offset) const
{
	// get buffer id
	ID bufferId = _buffers.at(name).at(frame);

	// set buffer
	_renderer.set_buffer(bufferId, value, offset, size);
}

void minty::rendering::DescriptorSet::set(String const& name, Dynamic const& value, VkDeviceSize const offset) const
{
	auto const& bufferIds = _buffers.at(name);

	for (auto const id : bufferIds)
	{
		_renderer.set_buffer(id, value.data(), static_cast<VkDeviceSize>(value.size()), offset);
	}
}

void minty::rendering::DescriptorSet::set(String const& name, int const frame, Dynamic const& value, VkDeviceSize const offset) const
{
	ID bufferId = _buffers.at(name).at(frame);

	_renderer.set_buffer(bufferId, value.data(), static_cast<VkDeviceSize>(value.size()), offset);
}

void minty::rendering::DescriptorSet::get(String const& name, int const frame, void* const out) const
{
	ID bufferId = _buffers.at(name).at(frame);

	_renderer.get_buffer_data(bufferId, out);
}

VkDescriptorSet minty::rendering::DescriptorSet::at(uint32_t const index) const
{
	return _descriptorSets.at(index);
}

std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> const& minty::rendering::DescriptorSet::data() const
{
	return _descriptorSets;
}

auto minty::rendering::DescriptorSet::begin()
{
	return _descriptorSets.begin();
}

auto minty::rendering::DescriptorSet::begin() const
{
	return _descriptorSets.begin();
}

auto minty::rendering::DescriptorSet::end()
{
	return _descriptorSets.end();
}

auto minty::rendering::DescriptorSet::end() const
{
	return _descriptorSets.end();
}
