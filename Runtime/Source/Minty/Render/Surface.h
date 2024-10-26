#pragma once

#include "Minty/Core/Pointer.h"
#include "Minty/Window/Window.h"

namespace Minty
{
	struct SurfaceBuilder
	{
		Ref<Window> window;
	};

	class Surface
	{
	private:

	protected:
		Surface() = default;

	public:
		virtual ~Surface() = default;

	public:
		static Owner<Surface> create(const SurfaceBuilder& builder = {});
	};
}