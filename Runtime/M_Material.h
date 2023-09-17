#pragma once
#include "M_Object.h"

#include "M_Texture.h"

namespace minty
{
	class Material :
		public Object
	{
	public: // TODO: TEMP PUBLIC
		Texture* const _texture;
	public:
		Material(Texture* const tex);
	};
}