#pragma once

#include "M_Base.h"

namespace minty::encoding
{
	std::string encode_base64(std::string const& text);

	std::string decode_base64(std::string const& text);
}