#pragma once

#include "M_Macros.h"

namespace Minty
{
	constexpr char const* ASSEMBLY_ENGINE_NAME = "MintyEngine";
	constexpr char const* ASSEMBLY_EDITOR_NAME = "MintyEditor";
	constexpr char const* SCRIPT_NAMESPACE_NAME = "MintyEngine";

	constexpr char const* MINTY_NAME = "Minty Engine";
	constexpr uint32_t const MINTY_VERSION = MINTY_MAKE_VERSION(0, 1, 0);
	constexpr uint32_t const MINTY_API_VERSION = MINTY_MAKE_VERSION(1, 2, 0);

	constexpr char const* BLANK = "";

	constexpr int MAX_WINDOW_COUNT = 1;

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

	constexpr char const* SCRIPT_METHOD_NAME_ONCREATE = "OnCreate";
	constexpr char const* SCRIPT_METHOD_NAME_ONLOAD = "OnLoad";
	constexpr char const* SCRIPT_METHOD_NAME_ONENABLE = "OnEnable";
	constexpr char const* SCRIPT_METHOD_NAME_ONUPDATE = "OnUpdate";
	constexpr char const* SCRIPT_METHOD_NAME_ONDISABLE = "OnDisable";
	constexpr char const* SCRIPT_METHOD_NAME_ONUNLOAD = "OnUnload";
	constexpr char const* SCRIPT_METHOD_NAME_ONDESTROY = "OnDestroy";

	constexpr char const* SCRIPT_METHOD_NAME_ONPOINTERENTER = "OnPointerEnter";
	constexpr char const* SCRIPT_METHOD_NAME_ONPOINTERHOVER = "OnPointerHover";
	constexpr char const* SCRIPT_METHOD_NAME_ONPOINTEREXIT = "OnPointerExit";
	constexpr char const* SCRIPT_METHOD_NAME_ONPOINTERMOVE = "OnPointerMove";
	constexpr char const* SCRIPT_METHOD_NAME_ONPOINTERDOWN = "OnPointerDown";
	constexpr char const* SCRIPT_METHOD_NAME_ONPOINTERUP = "OnPointerUp";
	constexpr char const* SCRIPT_METHOD_NAME_ONPOINTERCLICK = "OnPointerClick";

	constexpr char const* SCRIPT_INPUT_TRIGGER_KEY = "TriggerKey";
	constexpr char const* SCRIPT_INPUT_TRIGGER_MOUSE_CLICK = "TriggerMouseClick";
	constexpr char const* SCRIPT_INPUT_TRIGGER_MOUSE_MOVE = "TriggerMouseMove";
	constexpr char const* SCRIPT_INPUT_TRIGGER_MOUSE_SCROLL = "TriggerMouseScroll";
	constexpr char const* SCRIPT_INPUT_TRIGGER_GAMEPAD_CONNECT = "TriggerGamepadConnect";
	constexpr char const* SCRIPT_INPUT_TRIGGER_GAMEPAD_DISCONNECT = "TriggerGamepadDisconnect";
	constexpr char const* SCRIPT_INPUT_TRIGGER_GAMEPAD_BUTTON = "TriggerGamepadButton";
	constexpr char const* SCRIPT_INPUT_TRIGGER_GAMEPAD_AXIS = "TriggerGamepadAxis";

	constexpr char const* EXTENSION_APPLICATION_DATA = ".appdata";
	constexpr char const* EXTENSION_META = ".meta";
	constexpr char const* EXTENSION_WRAP = ".wrap";
}