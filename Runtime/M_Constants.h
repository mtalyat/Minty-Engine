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

	constexpr unsigned int DESCRIPTOR_SET_INVALID = -1;

	constexpr unsigned int DESCRIPTOR_SET_SHADER = 0;
	constexpr unsigned int DESCRIPTOR_SET_SHADER_PASS = 1;
	constexpr unsigned int DESCRIPTOR_SET_MATERIAL = 2;

	constexpr char const* BLANK = "_";

	constexpr char const* META_EXTENSION = ".mmeta";
	constexpr char const* ANIMATION_EXTENSION = ".manimation";
	constexpr char const* ANIMATOR_EXTENSION = ".manimator";
	constexpr char const* SCENE_EXTENSION = ".mscene";
	constexpr char const* SPRITE_EXTENSION = ".msprite";
	constexpr char const* MATERIAL_EXTENSION = ".mmaterial";
	constexpr char const* MATERIAL_TEMPLATE_EXTENSION = ".mmaterialtemplate";
	constexpr char const* SHADER_PASS_EXTENSION = ".mshaderpass";
	constexpr char const* SHADER_EXTENSION = ".mshader";
	constexpr char const* WRAP_EXTENSION = ".wrap";
}