#include "pch.h"
#include "Minty/Rendering/M_DescriptorSet.h"

#include "Minty/Types/M_Dynamic.h"
#include "Minty/Rendering/M_RenderEngine.h"
#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Scenes/M_Scene.h"
#include "Minty/Rendering/M_RenderSystem.h"

using namespace Minty;
using namespace Minty;

Minty::DescriptorSet::DescriptorSet(DescriptorSetBuilder const& builder)
	: RenderObject()
	, _descriptorPool(builder.pool)
	, _set(builder.set)
	, _descriptorSets(builder.descriptorSets)
	, _descriptors(builder.datas)
	, _dirties()
{
	// dirty all frames on the start so that they can all be set
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		_dirties.emplace(i);
	}
}

DescriptorSet& Minty::DescriptorSet::operator=(DescriptorSet const& other)
{
	if (&other != this)
	{
		_descriptorSets = other._descriptorSets;
		_descriptors = other._descriptors;
	}

	return *this;
}

void Minty::DescriptorSet::destroy()
{
	RenderEngine& renderer = RenderEngine::instance();
	AssetEngine& assets = AssetEngine::instance();

	// free using shader, to clean up resources there
	//if(_shader) _shader->free_descriptor_set(*this);

	// remove references to all VK descriptor sets, since they not need be destroyed
	for (size_t i = 0; i < _descriptorSets.size(); i++)
	{
		_descriptorSets[i] = VK_NULL_HANDLE;
	}

	// TODO: assess, then remove, or fix
	// COMMENTED OUT: problem is that other materials might share resources, so...
	// let the scene just destroy all of the material (buffers) later
	//
	// destroy all buffers for each descriptor
	//for (auto const& data : _descriptors)
	//{
	//	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	//	{
	//		for (auto const id : data.second.at(i).ids)
	//		{
	//			if (Ref<Buffer> buffer = assets.get<Buffer>(id))
	//			{
	//				
	//				//renderer.destroy_buffer(*buffer);
	//			}
	//		}
	//	}
	//}

	_descriptorPool = VK_NULL_HANDLE;
	_set = 0;
	_descriptors.clear();
	_dirties.clear();
}

void Minty::DescriptorSet::set(String const& name, void const* const value)
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

void Minty::DescriptorSet::set(String const& name, int const frame, void const* const value)
{
	if (DescriptorData* data = find_descriptor(name, frame))
	{
		// assuming size is the same as the buffer
		set_descriptor(*data, frame, value, 0, 0);
	}
}

void Minty::DescriptorSet::set(String const& name, void const* const value, VkDeviceSize const size, VkDeviceSize const offset)
{
	if (auto* datas = find_descriptors(name))
	{
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			set_descriptor(datas->at(i), i, value, size, offset);
		}
	}
}

void Minty::DescriptorSet::set(String const& name, int const frame, void const* const value, VkDeviceSize const size, VkDeviceSize const offset)
{
	if (DescriptorData* data = find_descriptor(name, frame))
	{
		set_descriptor(*data, frame, value, size, offset);
	}
}

void Minty::DescriptorSet::set(String const& name, Dynamic const& value, VkDeviceSize const offset)
{
	if (auto* datas = find_descriptors(name))
	{
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			set_descriptor(datas->at(i), i, value.data(), value.size(), offset);
		}
	}
}

void Minty::DescriptorSet::set(String const& name, int const frame, Dynamic const& value, VkDeviceSize const offset)
{
	if (DescriptorData* data = find_descriptor(name, frame))
	{
		set_descriptor(*data, frame, value.data(), value.size(), offset);
	}
}

void Minty::DescriptorSet::apply(int const frame)
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

	RenderEngine& renderer = RenderEngine::instance();
	AssetEngine& assets = AssetEngine::instance();
	RenderSystem* renderSystem = get_render_system();

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
			UUID bufferId = data.ids.at(0);

			// add buffer info(s)
			bufferInfos.push_back(VkDescriptorBufferInfo());

			// set buffer info
			Ref<Buffer> buffer = assets.at<Buffer>(bufferId);
			VkDescriptorBufferInfo& bufferInfo = bufferInfos.back();
			bufferInfo.buffer = buffer->get_buffer();
			bufferInfo.offset = 0;
			bufferInfo.range = buffer->get_size();

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
				UUID textureId = data.ids.at(i);

				Ref<Texture> texture = assets.at<Texture>(textureId);

				VkDescriptorImageInfo& info = infos.at(i);

				info.sampler = texture->get_sampler();
				info.imageView = texture->get_image_view();
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

void Minty::DescriptorSet::apply()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		apply(i);
	}
}

std::array<DescriptorData, MAX_FRAMES_IN_FLIGHT>* Minty::DescriptorSet::find_descriptors(String const& name)
{
	auto found = _descriptors.find(name);

	if (found == _descriptors.end())
	{
		return nullptr;
	}

	return &found->second;
}

DescriptorData* Minty::DescriptorSet::find_descriptor(String const& name, int const frame)
{
	if (auto* descriptors = find_descriptors(name))
	{
		return &descriptors->at(frame);
	}

	return nullptr;
}

void Minty::DescriptorSet::set_descriptor(DescriptorData& data, int const frame, void const* const value, VkDeviceSize const size, VkDeviceSize const offset)
{
	AssetEngine& assets = AssetEngine::instance();

	// do something based on type
	switch (data.type)
	{
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
	{
		// get buffer
		UUID bufferId = data.ids.front();
		Ref<Buffer> buffer = assets.at<Buffer>(bufferId);

		// set buffer
		RenderEngine& renderer = RenderEngine::instance();
		renderer.set_buffer(buffer, value, size, offset);

		break;
	}
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
	{
		// assume the input for this was an array of IDs
		UUID const* ids = static_cast<UUID const*>(value);
		size_t count = size / sizeof(UUID);

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

bool Minty::DescriptorSet::get(String const& name, int const frame, void* const out) const
{
	AssetEngine& assets = AssetEngine::instance();

	auto found = _descriptors.find(name);

	if (found == _descriptors.end())
	{
		return false;
	}

	switch (found->second.at(frame).type)
	{
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
	{
		// get buffer id
		UUID bufferId = found->second.at(frame).ids.front();
		Ref<Buffer> buffer = assets.at<Buffer>(bufferId);

		// set the data
		RenderEngine& renderer = RenderEngine::instance();
		renderer.get_buffer_data(buffer, out);

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

VkDescriptorSet Minty::DescriptorSet::at(uint32_t const index) const
{
	return _descriptorSets.at(index);
}

std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> const& Minty::DescriptorSet::data() const
{
	return _descriptorSets;
}

auto Minty::DescriptorSet::begin()
{
	return _descriptorSets.begin();
}

auto Minty::DescriptorSet::begin() const
{
	return _descriptorSets.begin();
}

auto Minty::DescriptorSet::end()
{
	return _descriptorSets.end();
}

auto Minty::DescriptorSet::end() const
{
	return _descriptorSets.end();
}
