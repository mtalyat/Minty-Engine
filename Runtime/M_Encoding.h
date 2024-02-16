#pragma once

#include "M_Base.h"
#include "M_Dynamic.h"

namespace minty::Encoding
{
	String encode_base64(Dynamic const& data);

	String encode_base64(void const* const data, size_t const size);

	template<typename T>
	String encode_base64(T const& t)
	{
		return encode_base64(&t, sizeof(T));
	}

	Dynamic decode_base64(String const& text);

	template<typename T>
	T decode_base64(String const& text)
	{
		return decode_base64(text).get<T>();
	}

	bool is_base64(String const& text);
}