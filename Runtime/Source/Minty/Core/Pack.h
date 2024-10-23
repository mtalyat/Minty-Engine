#pragma once
#include "Minty/Core/Types.h"

namespace Minty
{
	template<typename T>
	void pack_into(Byte* const data, Size& offset, const T& value)
	{
		memcpy(data + offset, &value, sizeof(T));
		offset += sizeof(T);
	}

	template<typename... Args>
	void pack(Byte* const data, const Size size, const Args&... args)
	{
		Size offset = 0;
		(pack_into(data, offset, args), ...);
	}

	template<typename... Args>
	Byte* pack_new(const Args&... args)
	{
		Size size = (sizeof(Args) + ...);
		Byte* data = new Byte[size];
		Minty::pack(data, size, args...);
		return data;
	}
}