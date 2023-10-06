#pragma once
#include "M_Object.h"

#include "M_Color.h"
#include <unordered_map>

namespace minty
{
	struct MaterialData
	{
		void* data;
		size_t size;
	};
}

namespace minty::rendering
{
	class MaterialBuilder
		: public Object
	{
	private:
		ID _shaderId;

		std::unordered_map<std::string, MaterialData> _values;
	public:
		MaterialBuilder(ID const shaderId = ERROR_ID);

		~MaterialBuilder();

#pragma region Set

		void set_shader_id(ID const id);

		void set(std::string const& name, void* const data, size_t const size);

#pragma endregion

#pragma region Get

		ID get_shader_id() const;

		std::unordered_map<std::string, MaterialData> const& get_values() const;

#pragma endregion
	};
}