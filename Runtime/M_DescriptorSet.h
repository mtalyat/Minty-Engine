#pragma once
#include "M_RenderObject.h"

#include "M_Vulkan.h"
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace minty
{
	class Dynamic;

	/// <summary>
	/// Holds data for a descriptor set within a shader.
	/// </summary>
	class DescriptorSet
		: public RenderObject
	{
	public:
		struct DescriptorData
		{
			/// <summary>
			/// Is this descriptor empty or not?
			/// </summary>
			bool empty;

			/// <summary>
			/// The type of this descriptor.
			/// </summary>
			VkDescriptorType type;

			/// <summary>
			/// The binding for this descriptor, within its set.
			/// </summary>
			uint32_t binding;

			/// <summary>
			/// The number of descriptors, if an array.
			/// </summary>
			uint32_t count;

			/// <summary>
			// IDs for buffers, textures, whatever is needed for this Descriptor.
			/// </summary>
			std::vector<UUID> ids;
		};

	private:
		std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> _descriptorSets;

		std::unordered_map<String, std::array<DescriptorData, MAX_FRAMES_IN_FLIGHT>> _descriptors;

		// if dirty, when applied, a descriptor write must occur
		std::unordered_set<int> _dirties;

	public:
		/// <summary>
		/// Creates an empty DescriptorSet.
		/// </summary>
		DescriptorSet();

		/// <summary>
		/// Creates a new DescriptorSet.
		/// </summary>
		/// <param name="renderer"></param>
		DescriptorSet(Runtime& engine);

		/// <summary>
		/// Creates a new DescriptorSet.
		/// </summary>
		/// <param name="descriptorSets">The descriptor sets for one flight.</param>
		/// <param name="buffers">The buffers associated with this DescriptorSet.</param>
		/// <param name="renderer"></param>
		DescriptorSet(std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> const& descriptorSets, std::unordered_map<String, std::array<DescriptorData, MAX_FRAMES_IN_FLIGHT>> const& datas, Runtime& engine);

		// copy
		DescriptorSet& operator=(DescriptorSet const& other);

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
		void set(String const& name, void const* const value);

		/// <summary>
		/// Sets the value of the buffer with the given name for one frame in the flight.
		/// </summary>
		/// <param name="name">The name of the buffer.</param>
		/// <param name="frame">The frame to set the values on.</param>
		/// <param name="value">The value to set.</param>
		void set(String const& name, int const frame, void const* const value);

		/// <summary>
		/// Sets the value of the buffer with the given name for all frames in the flight.
		/// </summary>
		/// <param name="name">The name of the buffer.</param>
		/// <param name="value">The value to set.</param>
		/// <param name="size">The number of bytes to set.</param>
		/// <param name="offset">The offset in the buffer in bytes.</param>
		void set(String const& name, void const* const value, VkDeviceSize const size, VkDeviceSize const offset = 0);

		/// <summary>
		/// Sets the value of the buffer with the given name for one frame in the flight.
		/// </summary>
		/// <param name="name">The name of the buffer.</param>
		/// <param name="frame">The frame to set the values on.</param>
		/// <param name="value">The value to set.</param>
		/// <param name="size">The number of bytes to set.</param>
		/// <param name="offset">The offset in the buffer in bytes.</param>
		void set(String const& name, int const frame, void const* const value, VkDeviceSize const size, VkDeviceSize const offset = 0);

		/// <summary>
		/// Sets the value of the buffer with the given name for all frames in the flight.
		/// </summary>
		/// <param name="name">The name of the buffer.</param>
		/// <param name="value">The value to set.</param>
		/// <param name="offset">The offset in the buffer in bytes.</param>
		void set(String const& name, Dynamic const& value, VkDeviceSize const offset = 0);

		/// <summary>
		/// Sets the value of the buffer with the given name for one frame in the flight.
		/// </summary>
		/// <param name="name">The name of the buffer.</param>
		/// <param name="frame">The frame to set the values on.</param>
		/// <param name="value">The value to set.</param>
		/// <param name="offset">The offset in the buffer in bytes.</param>
		void set(String const& name, int const frame, Dynamic const& value, VkDeviceSize const offset = 0);

		/// <summary>
		/// Applies all of the changes for the given frame to the GPU.
		/// </summary>
		/// <param name="frame"></param>
		void apply(int const frame);

		/// <summary>
		/// Applies all set changes to the GPU.
		/// </summary>
		void apply();
	private:
		std::array<DescriptorData, MAX_FRAMES_IN_FLIGHT>* find_descriptors(String const& name);

		// finds the descriptor with the given name
		DescriptorData* find_descriptor(String const& name, int const frame);

		// sets (updates) the given descriptor with the given value data and size for the given frame
		void set_descriptor(DescriptorData& data, int const frame, void const* const value, VkDeviceSize const size, VkDeviceSize const offset);

#pragma endregion

#pragma region Get

	public:
		/// <summary>
		/// Gets the value of a buffer and sets it to the out pointer.
		/// </summary>
		/// <param name="name">The name of the buffer.</param>
		/// <param name="frame">The frame to get the values from.</param>
		/// <param name="out">The pointer to the location to copy the values to.</param>
		/// <returns>True if the corresponding buffer was found.</returns>
		bool get(String const& name, int const frame, void* const out) const;

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