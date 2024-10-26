#pragma once

#include "Minty/Core/String.h"
#include "Minty/Core/Types.h"

namespace Minty::Debug
{
	void log(String const& text);

	void log_warning(String const& text);

	void log_error(String const& text);

	void log_info(String const& text);

	void log_todo(String const& text);

	void log_stack_trace();
}