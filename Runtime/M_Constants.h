#pragma once

namespace minty
{
	/// <summary>
	/// Defines how many frames are within one flight when sent to the GPU device.
	/// </summary>
	constexpr int MAX_FRAMES_IN_FLIGHT = 2;

	/// <summary>
	/// Number of descriptor sets that can fit within a descriptor pool.
	/// </summary>
	constexpr int DESCRIPTOR_SETS_PER_POOL = 4;

	/// <summary>
	/// The number of descriptor sets in the engine.
	/// </summary>
	constexpr int DESCRIPTOR_SET_COUNT = 3;

	constexpr unsigned int DESCRIPTOR_SET_SHADER = 0;
	constexpr unsigned int DESCRIPTOR_SET_SHADER_PASS = 1;
	constexpr unsigned int DESCRIPTOR_SET_MATERIAL = 2;
}