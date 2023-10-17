#pragma once
#include "M_Object.h"

namespace minty
{
	class Renderer;
}

namespace minty::rendering
{
	class RendererObject :
		public Object
	{
	protected:
		minty::Renderer& _renderer;

	public:
		RendererObject(minty::Renderer& renderer);

		virtual ~RendererObject();
	};
}

