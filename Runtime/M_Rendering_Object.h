#pragma once
#include "M_Object.h"

namespace minty
{
	class RenderEngine;
	class RenderSystem;
}

namespace minty::rendering
{
	/// <summary>
	/// Base class for an object that is used with the RenderEngine.
	/// </summary>
	class RenderObject :
		public Object
	{
	private:
		minty::RenderEngine* _renderer;

	public:
		RenderObject();

		/// <summary>
		/// Creates a new RenderObject.
		/// </summary>
		/// <param name="renderer"></param>
		RenderObject(RenderEngine& renderer);

		virtual ~RenderObject();

		void set_render_engine(RenderEngine& renderer);

		RenderEngine& get_render_engine() const;

		RenderSystem* get_render_system() const;

		friend String to_string(RenderObject const& value);
	};
}

