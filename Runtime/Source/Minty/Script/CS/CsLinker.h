#pragma once
#include "CS.h"

#include "Minty/Core/Type.h"

namespace Minty
{
	class CsLinker
	{
	protected:
		CsLinker() = default;
		~CsLinker() = default;

	public:
		static void link();
	};
}