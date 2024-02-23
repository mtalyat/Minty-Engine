#pragma once

namespace minty
{
	constexpr static char const* ASSEMBLY_ENGINE_NAME = "MintyEngine";
	constexpr static char const* ASSEMBLY_EDITOR_NAME = "MintyEditor";

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

	constexpr char const* SCRIPT_METHOD_NAME_ONCREATE = "OnCreate";
	constexpr char const* SCRIPT_METHOD_NAME_ONLOAD = "OnLoad";
	constexpr char const* SCRIPT_METHOD_NAME_ONENABLE = "OnEnable";
	constexpr char const* SCRIPT_METHOD_NAME_ONUPDATE = "OnUpdate";
	constexpr char const* SCRIPT_METHOD_NAME_ONDISABLE = "OnDisable";
	constexpr char const* SCRIPT_METHOD_NAME_ONUNLOAD = "OnUnload";
	constexpr char const* SCRIPT_METHOD_NAME_ONDESTROY = "OnDestroy";

	constexpr static char const* SCRIPT_INPUT_TRIGGER_KEY = "TriggerKey";
	constexpr static char const* SCRIPT_INPUT_TRIGGER_MOUSE_CLICK = "TriggerMouseClick";
	constexpr static char const* SCRIPT_INPUT_TRIGGER_MOUSE_MOVE = "TriggerMouseMove";
	constexpr static char const* SCRIPT_INPUT_TRIGGER_MOUSE_SCROLL = "TriggerMouseScroll";
}