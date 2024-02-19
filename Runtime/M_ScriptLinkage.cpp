#include "pch.h"
#include "M_ScriptLinkage.h"

#include "M_Constants.h"
#include "M_Console.h"
#include "M_Mono.h"

using namespace minty;

constexpr static char const* INTERNAL_CLASS_NAME = "Runtime";

#define ADD_INTERNAL_CALL(csharpName, cppName) mono_add_internal_call(std::format("{}.{}::{}", ASSEMBLY_ENGINE_NAME, INTERNAL_CLASS_NAME, csharpName).c_str(), cppName)

static String mono_string_to_string(MonoString* const string)
{
	char* str = mono_string_to_utf8(string);
	std::string result(str);
	mono_free(str);
	return result;
}

static void console_log(MonoString* string)
{
	Console::log(mono_string_to_string(string));
}

static void console_log_color(MonoString* string, int color)
{
	Console::log_color(mono_string_to_string(string), static_cast<Console::Color>(color));
}

static void console_warn(MonoString* string)
{
	Console::warn(mono_string_to_string(string));
}

static void console_error(MonoString* string)
{
	Console::error(mono_string_to_string(string));
}

static void console_ass(bool condition, MonoString* string)
{
	Console::ass(condition, mono_string_to_string(string));
}

void minty::ScriptLinkage::link()
{
	ADD_INTERNAL_CALL("Console_Log", console_log);
	ADD_INTERNAL_CALL("Console_LogColor", console_log_color);
	ADD_INTERNAL_CALL("Console_Warn", console_warn);
	ADD_INTERNAL_CALL("Console_Error", console_error);
	ADD_INTERNAL_CALL("Console_Assert", console_ass);
}

#undef ADD_INTERNAL_CALL