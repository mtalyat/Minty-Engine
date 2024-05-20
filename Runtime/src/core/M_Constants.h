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

	constexpr static char const* BLANK = "";

	constexpr static char const* SCRIPT_METHOD_NAME_ONCREATE = "OnCreate";
	constexpr static char const* SCRIPT_METHOD_NAME_ONLOAD = "OnLoad";
	constexpr static char const* SCRIPT_METHOD_NAME_ONENABLE = "OnEnable";
	constexpr static char const* SCRIPT_METHOD_NAME_ONUPDATE = "OnUpdate";
	constexpr static char const* SCRIPT_METHOD_NAME_ONDISABLE = "OnDisable";
	constexpr static char const* SCRIPT_METHOD_NAME_ONUNLOAD = "OnUnload";
	constexpr static char const* SCRIPT_METHOD_NAME_ONDESTROY = "OnDestroy";

	constexpr static char const* SCRIPT_METHOD_NAME_ONPOINTERENTER = "OnPointerEnter";
	constexpr static char const* SCRIPT_METHOD_NAME_ONPOINTERHOVER = "OnPointerHover";
	constexpr static char const* SCRIPT_METHOD_NAME_ONPOINTEREXIT = "OnPointerExit";
	constexpr static char const* SCRIPT_METHOD_NAME_ONPOINTERMOVE = "OnPointerMove";
	constexpr static char const* SCRIPT_METHOD_NAME_ONPOINTERDOWN = "OnPointerDown";
	constexpr static char const* SCRIPT_METHOD_NAME_ONPOINTERUP = "OnPointerUp";
	constexpr static char const* SCRIPT_METHOD_NAME_ONPOINTERCLICK = "OnPointerClick";

	constexpr static char const* SCRIPT_INPUT_TRIGGER_KEY = "TriggerKey";
	constexpr static char const* SCRIPT_INPUT_TRIGGER_MOUSE_CLICK = "TriggerMouseClick";
	constexpr static char const* SCRIPT_INPUT_TRIGGER_MOUSE_MOVE = "TriggerMouseMove";
	constexpr static char const* SCRIPT_INPUT_TRIGGER_MOUSE_SCROLL = "TriggerMouseScroll";
	constexpr static char const* SCRIPT_INPUT_TRIGGER_GAMEPAD_CONNECT = "TriggerGamepadConnect";
	constexpr static char const* SCRIPT_INPUT_TRIGGER_GAMEPAD_DISCONNECT = "TriggerGamepadDisconnect";
	constexpr static char const* SCRIPT_INPUT_TRIGGER_GAMEPAD_BUTTON = "TriggerGamepadButton";
	constexpr static char const* SCRIPT_INPUT_TRIGGER_GAMEPAD_AXIS = "TriggerGamepadAxis";

	constexpr static char const* EXTENSION_APPLICATION_DATA = ".appdata";
	constexpr static char const* EXTENSION_META = ".meta";
	constexpr static char const* EXTENSION_WRAP = ".wrap";
}