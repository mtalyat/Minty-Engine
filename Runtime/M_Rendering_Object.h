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
	protected:
		minty::RenderEngine& _renderer;

	public:
		/// <summary>
		/// Creates a new RenderObject.
		/// </summary>
		/// <param name="renderer"></param>
		RenderObject(minty::RenderEngine& renderer);

		virtual ~RenderObject();

		friend std::string to_string(RenderObject const& value);
	};
}

