#pragma once
#include "M_Object.h"

namespace minty::rendering
{
	class MaterialBuilder
		: public Object
	{
	private:
		ID _shaderId;

	public:
		MaterialBuilder(ID const shaderId);

		void set_shader_id(ID const id);

		ID get_shader_id() const;

		std::string to_string(MaterialBuilder const& value);
	};
}