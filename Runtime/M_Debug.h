#pragma once
#include "M_Types.h"

namespace minty::Debug
{
	void log(String const& text);

	void log_warning(String const& text);

	void log_error(String const& text);

	void log_info(String const& text);

	void log_todo(String const& text);
}