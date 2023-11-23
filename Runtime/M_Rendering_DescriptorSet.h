#pragma once

#include "M_Rendering_Object.h"
#include "M_Dynamic.h"

#include <array>
#include <unordered_map>

namespace minty::rendering
{
	class DescriptorSet
		: public RendererObject
	{
	private:
		std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> _descriptorSets;

		std::unordered_map<std::string, std::array<ID, MAX_FRAMES_IN_FLIGHT>> _buffers;

	public:
		DescriptorSet(Renderer& renderer);

		DescriptorSet(std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> const& descriptorSets, std::unordered_map<std::string, std::array<ID, MAX_FRAMES_IN_FLIGHT>> const& buffers, Renderer& renderer);

		void operator=(DescriptorSet const& other);

		void destroy();

#pragma region Set

		void set(std::string const& name, void* const value) const;

		void set(std::string const& name, int const frame, void* const value) const;

		void set(std::string const& name, void* const value, VkDeviceSize const size, VkDeviceSize const offset = 0) const;

		void set(std::string const& name, int const frame, void* const value, VkDeviceSize const size, VkDeviceSize const offset = 0) const;

		void set(std::string const& name, Dynamic const& value, VkDeviceSize const offset = 0) const;

		void set(std::string const& name, int const frame, Dynamic const& value, VkDeviceSize const offset = 0) const;

#pragma endregion

#pragma region Get

		void get(std::string const& name, int const frame, void* const out) const;

		VkDescriptorSet at(uint32_t const index) const;

		std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> const& data() const;

#pragma endregion

		auto begin();
		auto begin() const;
		auto end();
		auto end() const;
	};
}