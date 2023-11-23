#pragma once
#include "M_Object.h"

#include "M_Dynamic.h"
#include <unordered_map>

namespace minty::rendering
{
	class MaterialBuilder
		: public Object
	{
	private:
		ID _templateId;

		std::unordered_map<std::string, Dynamic> _values;

	public:
		MaterialBuilder(ID const templateId);

		void emplace_value(std::string const& name, Dynamic const& value);

		ID get_template_id() const;

		std::unordered_map<std::string, Dynamic> const& get_values() const;
	};
}