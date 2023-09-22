#pragma once

#include "M_Object.h"

namespace minty
{
	struct Component
		: public Object
	{

		/// <summary>
		/// Disposes any resources connected to this Component.
		/// </summary>
		virtual void dispose() {}
	};
}