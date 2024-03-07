#pragma once

namespace minty
{
	constexpr static char const* ASSEMBLY_ENGINE_NAME = "MintyEngine";
	constexpr static char const* ASSEMBLY_EDITOR_NAME = "MintyEditor";

	/// <summary>
	/// Defines how many frames are within one flight when sent to the GPU device.
	/// </summary>
	constexpr static int MAX_FRAMES_IN_FLIGHT = 2;

	/// <summary>
	/// Number of descriptor sets that can fit within a descriptor pool.
	/// </summary>
	constexpr static int DESCRIPTOR_SETS_PER_POOL = 4;

	/// <summary>
	/// The number of descriptor sets in the engine.
	/// </summary>
	constexpr static int DESCRIPTOR_SET_COUNT = 3;

	constexpr static unsigned int DESCRIPTOR_SET_INVALID = -1;

	constexpr static unsigned int DESCRIPTOR_SET_SHADER = 0;
	constexpr static unsigned int DESCRIPTOR_SET_SHADER_PASS = 1;
	constexpr static unsigned int DESCRIPTOR_SET_MATERIAL = 2;

	constexpr static char const* BLANK = "_";

	constexpr static char const* META_EXTENSION = ".mmeta";
	constexpr static char const* APPLICATION_EXTENSION = ".mapp";
	constexpr static char const* ANIMATION_EXTENSION = ".manimation";
	constexpr static char const* ANIMATOR_EXTENSION = ".manimator";
	constexpr static char const* SCENE_EXTENSION = ".mscene";
	constexpr static char const* SPRITE_EXTENSION = ".msprite";
	constexpr static char const* MATERIAL_EXTENSION = ".mmaterial";
	constexpr static char const* MATERIAL_TEMPLATE_EXTENSION = ".mmaterialtemplate";
	constexpr static char const* SHADER_PASS_EXTENSION = ".mshaderpass";
	constexpr static char const* SHADER_EXTENSION = ".mshader";
	constexpr static char const* WRAP_EXTENSION = ".wrap";
	constexpr static char const* SCRIPT_EXTENSION = ".cs";

	constexpr static char const* SCRIPT_METHOD_NAME_ONCREATE = "OnCreate";
	constexpr static char const* SCRIPT_METHOD_NAME_ONLOAD = "OnLoad";
	constexpr static char const* SCRIPT_METHOD_NAME_ONENABLE = "OnEnable";
	constexpr static char const* SCRIPT_METHOD_NAME_ONUPDATE = "OnUpdate";
	constexpr static char const* SCRIPT_METHOD_NAME_ONDISABLE = "OnDisable";
	constexpr static char const* SCRIPT_METHOD_NAME_ONUNLOAD = "OnUnload";
	constexpr static char const* SCRIPT_METHOD_NAME_ONDESTROY = "OnDestroy";

	constexpr static char const* SCRIPT_INPUT_TRIGGER_KEY = "TriggerKey";
	constexpr static char const* SCRIPT_INPUT_TRIGGER_MOUSE_CLICK = "TriggerMouseClick";
	constexpr static char const* SCRIPT_INPUT_TRIGGER_MOUSE_MOVE = "TriggerMouseMove";
	constexpr static char const* SCRIPT_INPUT_TRIGGER_MOUSE_SCROLL = "TriggerMouseScroll";
}