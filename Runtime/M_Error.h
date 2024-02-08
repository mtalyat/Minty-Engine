#pragma once

#include "M_Macros.h"
#include "M_Types.h"

#ifdef MINTY_RELEASE
#define MINTY_ASSERT(expression, message)
#else
#define MINTY_ASSERT(expression, message) if(!(expression)) minty::Error::abort(message);
#endif

#ifdef MINTY_RELEASE
#define MINTY_ERROR(expression, message)
#else
#define MINTY_ERROR(expression, message) if(!(expression)) minty::Console::error(message);
#endif

#ifdef MINTY_RELEASE
#define MINTY_WARN(expression, message)
#else
#define MINTY_WARN(expression, message) if(!(expression)) minty::Console::warn(message);
#endif

namespace minty::Error
{
	/// <summary>
	/// The default error text.
	/// </summary>
	constexpr char const* ERROR_TEXT = "ERROR";

	/// <summary>
	/// Aborts the program with the given error message.
	/// </summary>
	/// <param name="message"></param>
	void abort(minty::String const& message);
}