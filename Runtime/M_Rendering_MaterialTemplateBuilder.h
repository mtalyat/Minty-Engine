#pragma once
#include "M_Object.h"

#include "M_Dynamic.h"

#include <vector>
#include <unordered_map>

namespace minty::rendering
{
	class MaterialTemplateBuilder
		: public Object
	{
	private:
		std::vector<ID> _shaderPassIds;

		std::unordered_map<std::string, Dynamic> _defaultValues;

	public:
		MaterialTemplateBuilder(std::vector<ID> const& shaderPassIds);

#pragma region Set

		void emplace_default_value(std::string const& name, Dynamic const& value);

#pragma endregion

#pragma region Get

		std::vector<ID> const& get_shader_pass_ids() const;

		std::unordered_map<std::string, Dynamic> const& get_default_values() const;

#pragma endregion

	};
}