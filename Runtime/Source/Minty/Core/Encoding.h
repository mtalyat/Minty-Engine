#pragma once
#include "Minty/Core/Base.h"

#include "Minty/Core/String.h"
#include "Minty/Data/ConstantContainer.h"

namespace Minty::Encoding
{
#pragma region Base 64

	String encode_base64(void const* const data, Size const size);

	template<typename T>
	String encode_base64(T const& t)
	{
		return encode_base64(&t, sizeof(T));
	}

	Bool is_base64(String const& text);

#pragma endregion

#pragma region Base 16

	String encode_base16(void const* const data, Size const size);

	template<typename T>
	String encode_base16(T const& t)
	{
		return encode_base16(static_cast<void const* const>(&t), sizeof(T));
	}

	ConstantContainer decode_base16(String const& string);

	Bool is_base16(String const& text);

#pragma endregion
}