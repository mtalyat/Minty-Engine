#pragma once

#include "M_Rendering_Object.h"
#include "M_Dynamic.h"

#include <array>
#include <unordered_map>

namespace minty::rendering
{
	/// <summary>
	/// Holds data for a descriptor set within a shader.
	/// </summary>
	class DescriptorSet
		: public RenderObject
	{
	private:
		std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> _descriptorSets;

		std::unordered_map<String, std::array<ID, MAX_FRAMES_IN_FLIGHT>> _buffers;

	public:
		/// <summary>
		/// Creates a new DescriptorSet.
		/// </summary>
		/// <param name="renderer"></param>
		DescriptorSet(RenderEngine& renderer);

		/// <summary>
		/// Creates a new DescriptorSet.
		/// </summary>
		/// <param name="descriptorSets">The descriptor sets for one flight.</param>
		/// <param name="buffers">The buffers associated with this DescriptorSet.</param>
		/// <param name="renderer"></param>
		DescriptorSet(std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> const& descriptorSets, std::unordered_map<String, std::array<ID, MAX_FRAMES_IN_FLIGHT>> const& buffers, RenderEngine& renderer);

		// copy
		void operator=(DescriptorSet const& other);

		/// <summary>
		/// Destroys all resources associated with this DescriptorSet.
		/// </summary>
		void destroy();

#pragma region Set

		/// <summary>
		/// Sets the value of the buffer with the given name for all frames in the flight.
		/// </summary>
		/// <param name="name">The name of the buffer.</param>
		/// <param name="value">The value to set.</param>
		void set(String const& name, void* const value) const;

		/// <summary>
		/// Sets the value of the buffer with the given name for one frame in the flight.
		/// </summary>
		/// <param name="name">The name of the buffer.</param>
		/// <param name="frame">The frame to set the values on.</param>
		/// <param name="value">The value to set.</param>
		void set(String const& name, int const frame, void* const value) const;

		/// <summary>
		/// Sets the value of the buffer with the given name for all frames in the flight.
		/// </summary>
		/// <param name="name">The name of the buffer.</param>
		/// <param name="value">The value to set.</param>
		/// <param name="size">The number of bytes to set.</param>
		/// <param name="offset">The offset in the buffer in bytes.</param>
		void set(String const& name, void* const value, VkDeviceSize const size, VkDeviceSize const offset = 0) const;

		/// <summary>
		/// Sets the value of the buffer with the given name for one frame in the flight.
		/// </summary>
		/// <param name="name">The name of the buffer.</param>
		/// <param name="frame">The frame to set the values on.</param>
		/// <param name="value">The value to set.</param>
		/// <param name="size">The number of bytes to set.</param>
		/// <param name="offset">The offset in the buffer in bytes.</param>
		void set(String const& name, int const frame, void* const value, VkDeviceSize const size, VkDeviceSize const offset = 0) const;

		/// <summary>
		/// Sets the value of the buffer with the given name for all frames in the flight.
		/// </summary>
		/// <param name="name">The name of the buffer.</param>
		/// <param name="value">The value to set.</param>
		/// <param name="offset">The offset in the buffer in bytes.</param>
		void set(String const& name, Dynamic const& value, VkDeviceSize const offset = 0) const;

		/// <summary>
		/// Sets the value of the buffer with the given name for one frame in the flight.
		/// </summary>
		/// <param name="name">The name of the buffer.</param>
		/// <param name="frame">The frame to set the values on.</param>
		/// <param name="value">The value to set.</param>
		/// <param name="offset">The offset in the buffer in bytes.</param>
		void set(String const& name, int const frame, Dynamic const& value, VkDeviceSize const offset = 0) const;

#pragma endregion

#pragma region Get

		/// <summary>
		/// Gets the value of a buffer and sets it to the out pointer.
		/// </summary>
		/// <param name="name">The name of the buffer.</param>
		/// <param name="frame">The frame to get the values from.</param>
		/// <param name="out">The pointer to the location to copy the values to.</param>
		void get(String const& name, int const frame, void* const out) const;

		/// <summary>
		/// Gets the specific VkDescriptorSet at the given index.
		/// </summary>
		/// <param name="index">The index of the VkDescriptorSet.</param>
		/// <returns>The VkDescriptorSet at the index.</returns>
		VkDescriptorSet at(uint32_t const index) const;

		/// <summary>
		/// Gets the array of VkDescriptorSets for the flight.
		/// </summary>
		/// <returns></returns>
		std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> const& data() const;

#pragma endregion

		auto begin();
		auto begin() const;
		auto end();
		auto end() const;
	};
}