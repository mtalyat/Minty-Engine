#pragma once
#include "M_SceneObject.h"

namespace minty
{
	class RenderEngine;
	class RenderSystem;

	/// <summary>
	/// Base class for an object that is used with the RenderEngine.
	/// </summary>
	class RenderObject :
		public SceneObject
	{
		//friend class RenderEngine;
		//friend class RenderSystem;

	public:
		RenderObject();

		/// <summary>
		/// Creates a new RenderObject.
		/// </summary>
		/// <param name="renderer"></param>
		RenderObject(Engine& engine, ID const sceneId);

		virtual ~RenderObject();

		RenderEngine& get_render_engine() const;

		RenderSystem* get_render_system() const;

		friend String to_string(RenderObject const& value);
	};
}

