#pragma once
#include "Minty/Types/M_Object.h"

namespace Minty
{
	class AssetEngine;
	class RenderEngine;
	class RenderSystem;

	/// <summary>
	/// Base class for an object that is used with the RenderEngine.
	/// </summary>
	class RenderObject :
		public Object
	{
		//friend class RenderEngine;
		//friend class RenderSystem;

	protected:
		RenderObject() = default;
		virtual ~RenderObject() = default;

		RenderSystem* get_render_system() const;
	};
}

