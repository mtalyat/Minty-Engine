#include "pch.h"
#include "M_Rendering_DescriptorSet.h"

#include "M_RenderEngine.h"
#include "M_Scene.h"
#include "M_RenderSystem.h"

using namespace minty;
using namespace minty::rendering;

minty::rendering::DescriptorSet::DescriptorSet()
	: RenderObject::RenderObject()
	, _descriptorSets()
	, _descriptors()
	, _dirties()
{}

minty::rendering::DescriptorSet::DescriptorSet(RenderEngine& renderer)
	: RenderObject::RenderObject(renderer)
	, _descriptorSets()
	, _descriptors()
	, _dirties()
{}

minty::rendering::DescriptorSet::DescriptorSet(std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> const& descriptorSets, std::unordered_map<String, std::array<DescriptorData, MAX_FRAMES_IN_FLIGHT>> const& datas, RenderEngine& renderer)
	: RenderObject::RenderObject(renderer)
	, _descriptorSets(descriptorSets)
	, _descriptors(datas)
	, _dirties()
{
	// dirty all frames on the start so that they can all be set
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		_dirties.emplace(i);
	}
}

DescriptorSet& minty::rendering::DescriptorSet::operator=(DescriptorSet const& other)
{
	if (&other != this)
	{
		_descriptorSets = other._descriptorSets;
		_descriptors = other._descriptors;
	}

	return *this;
}

void minty::rendering::DescriptorSet::destroy()
{
	RenderEngine& renderer = get_render_engine();

	// remove references to all VK descriptor sets, since they not need be destroyed
	for (size_t i = 0; i < _descriptorSets.size(); i++)
	{
		_descriptorSets[i] = VK_NULL_HANDLE;
	}
	// destroy all buffers for each descriptor
	for (auto const& data : _descriptors)
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			for (auto const id : data.second.at(i).ids)
			{
				renderer.destroy_buffer(id);
			}
		}
	}
	_descriptors.clear();
}

void minty::rendering::DescriptorSet::set(String const& name, void const* const value)
{
	if (auto* datas = find_descriptors(name))
	{
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			// assuming size is the same as the buffer
			set_descriptor(datas->at(i), i, value, 0, 0);
		}
	}
}

void minty::rendering::DescriptorSet::set(String const& name, int const frame, void const* const value)
{
	if (DescriptorData* data = find_descriptor(name, frame))
	{
		// assuming size is the same as the buffer
		set_descriptor(*data, frame, value, 0, 0);
	}
}

void minty::rendering::DescriptorSet::set(String const& name, void const* const value, VkDeviceSize const size, VkDeviceSize const offset)
{
	if (auto* datas = find_descriptors(name))
	{
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			set_descriptor(datas->at(i), i, value, size, offset);
		}
	}
}

void minty::rendering::DescriptorSet::set(String const& name, int const frame, void const* const value, VkDeviceSize const size, VkDeviceSize const offset)
{
	if (DescriptorData* data = find_descriptor(name, frame))
	{
		set_descriptor(*data, frame, value, size, offset);
	}
}

void minty::rendering::DescriptorSet::set(String const& name, Dynamic const& value, VkDeviceSize const offset)
{
	if (auto* datas = find_descriptors(name))
	{
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			set_descriptor(datas->at(i), i, value.data(), value.size(), offset);
		}
	}
}

void minty::rendering::DescriptorSet::set(String const& name, int const frame, Dynamic const& value, VkDeviceSize const offset)
{
	if (DescriptorData* data = find_descriptor(name, frame))
	{
		set_descriptor(*data, frame, value.data(), value.size(), offset);
	}
}

void minty::rendering::DescriptorSet::apply(int const frame)
{
	if (!_dirties.contains(frame))
	{
		// this frame is not dirty, do not write descriptors
		return;
	}

	// mark as clean
	_dirties.erase(frame);

	// get the appropriate vk descriptor set for the frame
	VkDescriptorSet const& set = _descriptorSets.at(frame);

	std::vector<VkWriteDescriptorSet> writes;
	std::vector<VkDescriptorBufferInfo> bufferInfos;
	std::vector<std::vector<VkDescriptorImageInfo>> imageInfos;

	RenderEngine& renderer = get_render_engine();
	RenderSystem* renderSystem = renderer.get_scene()->get_system_registry().find<RenderSystem>();

	for (auto const& pair : _descriptors)
	{
		DescriptorData const& data = pair.second.at(frame);

		// if data is empty, skip it
		if (data.empty)
		{
			continue;
		}

		// create the write data
		writes.push_back(VkWriteDescriptorSet
			{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.pNext = nullptr,
				.dstSet = set,
				.dstBinding = data.binding,
				.dstArrayElement = 0,
				.descriptorCount = data.count,
				.descriptorType = data.type,
				.pImageInfo = nullptr,
				.pBufferInfo = nullptr,
				.pTexelBufferView = nullptr,
			});
		auto& write = writes.back();

		// do something based on type
		switch (data.type)
		{
		case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
		{
			// get buffer id
			ID bufferId = data.ids.at(0);

			// add buffer info(s)
			bufferInfos.push_back(VkDescriptorBufferInfo());

			// set buffer info
			VkDescriptorBufferInfo& bufferInfo = bufferInfos.back();
			bufferInfo.buffer = renderer.get_buffer(bufferId);
			bufferInfo.offset = 0;
			bufferInfo.range = renderer.get_buffer_size(bufferId);

			// add buffer info to write
			write.pBufferInfo = &bufferInfo;

			break;
		}
		case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
		{
			// create image info descriptor(s)
			imageInfos.push_back(std::vector<VkDescriptorImageInfo>(data.ids.size()));
			auto& infos = imageInfos.back();

			// populate with images based on ids
			for (size_t i = 0; i < data.ids.size(); i++)
			{
				ID textureId = data.ids.at(i);

				Texture const& texture = renderSystem->get_texture(textureId);

				VkDescriptorImageInfo& info = infos.at(i);

				info.sampler = texture.get_sampler();
				info.imageView = texture.get_image_view();
				info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			}

			// add image info to write
			write.pImageInfo = infos.data();

			break;
		}
		}
	}

	// apply all changes, if there were some
	if (uint32_t count = static_cast<uint32_t>(writes.size()))
	{
		vkUpdateDescriptorSets(renderer.get_device(), count, writes.data(), 0, nullptr);
	}
}

void minty::rendering::DescriptorSet::apply()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		apply(i);
	}
}

std::array<DescriptorSet::DescriptorData, MAX_FRAMES_IN_FLIGHT>* minty::rendering::DescriptorSet::find_descriptors(String const& name)
{
	auto const& found = _descriptors.find(name);

	if (found == _descriptors.end())
	{
		return nullptr;
	}

	return &found->second;
}

DescriptorSet::DescriptorData* minty::rendering::DescriptorSet::find_descriptor(String const& name, int const frame)
{
	if (auto* descriptors = find_descriptors(name))
	{
		return &descriptors->at(frame);
	}

	return nullptr;
}

void minty::rendering::DescriptorSet::set_descriptor(DescriptorData& data, int const frame, void const* const value, VkDeviceSize const size, VkDeviceSize const offset)
{
	// do something based on type
	switch (data.type)
	{
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
	{
		// get buffer
		ID bufferId = data.ids.front();

		// set buffer
		RenderEngine& renderer = get_render_engine();
		renderer.set_buffer(bufferId, value, size, offset);

		break;
	}
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
	{
		// assume the input for this was an array of IDs
		ID const* ids = static_cast<ID const*>(value);
		size_t count = size / sizeof(ID);

		// apply those to ids in data
		data.ids.resize(count);
		memcpy(data.ids.data(), ids, size);

		// mark set as dirty
		_dirties.emplace(frame);

		break;
	}
	default:
		// do nothing if not any of the above
		return;
	}

	// mark as not empty, since it has been set
	data.empty = false;
}

bool minty::rendering::DescriptorSet::get(String const& name, int const frame, void* const out) const
{
	auto const& found = _descriptors.find(name);

	if (found == _descriptors.end())
	{
		return false;
	}

	switch (found->second.at(frame).type)
	{
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
	{
		// get buffer id
		ID bufferId = found->second.at(frame).ids.front();

		// set the data
		RenderEngine& renderer = get_render_engine();
		renderer.get_buffer_data(bufferId, out);

		return true;
	}
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
	{
		// do nothing
		return false;
	}
	}

	// type not supported
	return false;
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
