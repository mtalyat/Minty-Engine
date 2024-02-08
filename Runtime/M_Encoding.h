#pragma once

#include "M_Base.h"
#include "M_Dynamic.h"

namespace minty::encoding
{
	String encode_base64(Dynamic const& data);

	Dynamic decode_base64(String const& text);
}