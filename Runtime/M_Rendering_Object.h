#pragma once
#include "M_Object.h"

namespace minty
{
	class RenderEngine;
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
		/// <summary>
		/// Creates a new RenderObject.
		/// </summary>
		/// <param name="renderer"></param>
		RenderObject(minty::RenderEngine* renderer = nullptr);

		virtual ~RenderObject();

		void set_renderer(RenderEngine* renderer);

		RenderEngine* get_renderer();

		RenderEngine const* get_renderer() const;

		friend String to_string(RenderObject const& value);
	};
}

