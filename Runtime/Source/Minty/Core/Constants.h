#pragma once

#include "Minty/Core/Macros.h"

namespace Minty
{
	constexpr Char const* MINTY_NAME_ENGINE = "MintyEngine";
	constexpr Char const* MINTY_NAME_EDITOR = "MintyEditor";
	constexpr Char const* MINTY_NAME_DOMAIN_APP = "MintyAppDomain";
	constexpr Char const* MINTY_NAME_DOMAIN_ROOT = "MintyRootDomain";
	constexpr Char const* MINTY_NAME_SCRIPT_NAMESPACE = "MintyEngine";

	constexpr Char const* MINTY_NAME = "Minty Engine";
	constexpr UInt const MINTY_VERSION = MINTY_MAKE_VERSION(0, 1, 0);

	constexpr Char const* BLANK = "";
	constexpr Char INDENT = '\t';

	constexpr Size MAX_WINDOW_COUNT = 1;

	// the number of frames passed to the GPU at one time
	constexpr Size MAX_FRAMES_IN_FLIGHT = 2;

	constexpr Char const* SCRIPT_METHOD_NAME_ONCREATE = "OnCreate";
	constexpr Char const* SCRIPT_METHOD_NAME_ONLOAD = "OnLoad";
	constexpr Char const* SCRIPT_METHOD_NAME_ONENABLE = "OnEnable";
	constexpr Char const* SCRIPT_METHOD_NAME_ONUPDATE = "OnUpdate";
	constexpr Char const* SCRIPT_METHOD_NAME_ONFINALIZE = "OnFinalize";
	constexpr Char const* SCRIPT_METHOD_NAME_ONDISABLE = "OnDisable";
	constexpr Char const* SCRIPT_METHOD_NAME_ONUNLOAD = "OnUnload";
	constexpr Char const* SCRIPT_METHOD_NAME_ONDESTROY = "OnDestroy";

	constexpr Char const* SCRIPT_METHOD_NAME_ONPOINTERENTER = "OnPointerEnter";
	constexpr Char const* SCRIPT_METHOD_NAME_ONPOINTERHOVER = "OnPointerHover";
	constexpr Char const* SCRIPT_METHOD_NAME_ONPOINTEREXIT = "OnPointerExit";
	constexpr Char const* SCRIPT_METHOD_NAME_ONPOINTERMOVE = "OnPointerMove";
	constexpr Char const* SCRIPT_METHOD_NAME_ONPOINTERDOWN = "OnPointerDown";
	constexpr Char const* SCRIPT_METHOD_NAME_ONPOINTERUP = "OnPointerUp";
	constexpr Char const* SCRIPT_METHOD_NAME_ONPOINTERCLICK = "OnPointerClick";

	constexpr Char const* SCRIPT_INPUT_TRIGGER_KEY = "TriggerKey";
	constexpr Char const* SCRIPT_INPUT_TRIGGER_MOUSE_CLICK = "TriggerMouseClick";
	constexpr Char const* SCRIPT_INPUT_TRIGGER_MOUSE_MOVE = "TriggerMouseMove";
	constexpr Char const* SCRIPT_INPUT_TRIGGER_MOUSE_SCROLL = "TriggerMouseScroll";
	constexpr Char const* SCRIPT_INPUT_TRIGGER_GAMEPAD_CONNECT = "TriggerGamepadConnect";
	constexpr Char const* SCRIPT_INPUT_TRIGGER_GAMEPAD_DISCONNECT = "TriggerGamepadDisconnect";
	constexpr Char const* SCRIPT_INPUT_TRIGGER_GAMEPAD_BUTTON = "TriggerGamepadButton";
	constexpr Char const* SCRIPT_INPUT_TRIGGER_GAMEPAD_AXIS = "TriggerGamepadAxis";

	constexpr Char const* EXTENSION_APPLICATION_DATA = ".appdata";
	constexpr Char const* EXTENSION_META = ".meta";
	constexpr Char const* EXTENSION_WRAP = ".wrap";

	constexpr Size INVALID_UUID = 0;
	constexpr UInt INVALID_HANDLE = UINT_MAX;

	// size of vertex
	constexpr Size DEFAULT_VERTEX_UNIT_COUNT = 8;
	constexpr Size DEFAULT_VERTEX_UNIT_SIZE = sizeof(Float);
	constexpr Size DEFAULT_VERTEX_SIZE = DEFAULT_VERTEX_UNIT_COUNT * DEFAULT_VERTEX_UNIT_SIZE;
	// size of indices
	constexpr Size DEFAULT_INDEX_UNIT_COUNT = 1;
	constexpr Size DEFAULT_INDEX_UNIT_SIZE = sizeof(UShort);
	constexpr Size DEFAULT_INDEX_SIZE = DEFAULT_INDEX_UNIT_COUNT * DEFAULT_INDEX_UNIT_SIZE;

	// TODO: update with correct IDs
	constexpr Size DEFAULT_SPRITE_SHADER = 0xdef000000000000b;
	constexpr Size DEFAULT_SPRITE_MATERIAL_TEMPLATE = 0xdef000000000000f;
	constexpr Size DEFAULT_TEXT_MATERIAL_TEMPLATE = 0xdef0000000000010;
	constexpr Size DEFAULT_UI_MATERIAL_TEMPLATE = 0xdef0000000000011;
	constexpr Size DEFAULT_TEXTURE = 0xdef0000000000000;
}