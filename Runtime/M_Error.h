#pragma once

#include <string>

#ifdef NDEBUG
#define MINTY_ASSERT(expression, message)
#else
#define MINTY_ASSERT(expression, message) if(!(expression)) minty::error::abort(message);
#endif

namespace minty::error
{
	constexpr char const* ERROR_TEXT = "ERROR";

	void abort(std::string const& message);
}