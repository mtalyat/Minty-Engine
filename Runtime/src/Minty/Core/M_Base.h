#pragma once

#include "Minty/Types/M_Types.h"
#include "Minty/Core/M_Macros.h"
#include "Minty/Core/M_Constants.h"
#include "Minty/Math/M_Math.h"
#include "Minty/Tools/M_Parse.h"
#include <format>

namespace Minty
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename... Args>
	constexpr Scope<T> create_scope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	
	template<typename T, typename... Args>
	constexpr Ref<T> create_ref(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr T* ref_to_pointer(Ref<T> const ref)
	{
		return &(*ref);
	}
}