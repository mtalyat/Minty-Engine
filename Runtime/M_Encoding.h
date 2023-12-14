#pragma once

#include "M_Base.h"

namespace minty::encoding
{
	/// <summary>
	/// Base64 encodes the given plain text.
	/// </summary>
	/// <param name="text">The text to encode.</param>
	/// <returns>The base64 encoded string.</returns>
	std::string encode_base64(std::string const& text);

	/// <summary>
	/// Base64 decodes the given text.
	/// </summary>
	/// <param name="text">The text to decode.</param>
	/// <returns>The plain text string.</returns>
	std::string decode_base64(std::string const& text);
}