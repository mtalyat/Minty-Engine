#pragma once

#include "Minty/Core/Macros.h"

namespace Minty
{
	constexpr char const* MINTY_NAME_ENGINE = "MintyEngine";
	constexpr char const* MINTY_NAME_EDITOR = "MintyEditor";
	constexpr char const* MINTY_NAME_DOMAIN = "MintyDomain";
	constexpr char const* MINTY_NAME_SCRIPT_NAMESPACE = "MintyEngine";

	constexpr char const* MINTY_NAME = "Minty Engine";
	constexpr uint32_t const MINTY_VERSION = MINTY_MAKE_VERSION(0, 1, 0);

	constexpr char const* BLANK = "";
	constexpr char INDENT = '\t';

	constexpr int MAX_WINDOW_COUNT = 1;

	// the number of frames passed to the GPU at one time
	constexpr Size MAX_FRAMES_IN_FLIGHT = 2;

	constexpr char const* SCRIPT_METHOD_NAME_ONCREATE = "OnCreate";
	constexpr char const* SCRIPT_METHOD_NAME_ONLOAD = "OnLoad";
	constexpr char const* SCRIPT_METHOD_NAME_ONENABLE = "OnEnable";
	constexpr char const* SCRIPT_METHOD_NAME_ONUPDATE = "OnUpdate";
	constexpr char const* SCRIPT_METHOD_NAME_ONFINALIZE = "OnFinalize";
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

	constexpr ULong INVALID_UUID = 0;
	constexpr UInt INVALID_HANDLE = UINT_MAX;
}