#include "pch.h"
#include "M_Error.h"

#include "M_Console.h"
#include <stdexcept>

using namespace minty;
using namespace minty::error;

void minty::error::abort(String const& message)
{
	// abort program
	throw std::runtime_error(message);
}