#pragma once

#include "M_Base.h"
#include "M_Dynamic.h"

namespace minty::Encoding
{
#pragma region Base 64

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

#pragma endregion

#pragma region Base 16

	String encode_base16(Dynamic const& data);

	String encode_base16(void const* const data, size_t const size);

	template<typename T>
	String encode_base16(T const& t)
	{
		return encode_base16(static_cast<void const* const>(&t), sizeof(T));
	}

	Dynamic decode_base16(String const& text);

	template<typename T>
	T decode_base16(String const& text)
	{
		return decode_base16(text).get<T>();
	}

	bool is_base16(String const& text);

#pragma endregion
}