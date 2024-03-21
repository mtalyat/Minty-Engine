#pragma once
#include "M_RuntimeObject.h"

namespace minty
{
	class AssetEngine;
	class RenderEngine;
	class RenderSystem;

	/// <summary>
	/// Base class for an object that is used with the RenderEngine.
	/// </summary>
	class RenderObject :
		public RuntimeObject
	{
		//friend class RenderEngine;
		//friend class RenderSystem;

	public:
		RenderObject();

		/// <summary>
		/// Creates a new RenderObject.
		/// </summary>
		/// <param name="renderer"></param>
		RenderObject(Runtime& engine);

		virtual ~RenderObject();

		AssetEngine& get_asset_engine() const;

		RenderEngine& get_render_engine() const;

		RenderSystem* get_render_system() const;

		friend String to_string(RenderObject const& value);
	};
}

