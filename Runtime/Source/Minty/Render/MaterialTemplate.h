#pragma once

#include "Minty/Asset/Asset.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Render/Shader.h"
#include <unordered_map>

namespace Minty
{
	struct MaterialTemplateBuilder
	{
		UUID id = {};
		Ref<Shader> shader;
		std::unordered_map<String, Cargo> values;
	};

	class MaterialTemplate
		: public Asset
	{
	private:
		Ref<Shader> m_shader;
		std::unordered_map<String, Cargo> m_values;

	public:
		MaterialTemplate(MaterialTemplateBuilder const& builder)
			: Asset(builder.id)
			, m_shader(builder.shader)
			, m_values(builder.values)
		{}

	public:
		virtual ~MaterialTemplate() = default;

	public:
		AssetType get_type() const override { return AssetType::MaterialTemplate; }

		Ref<Shader> get_shader() const { return m_shader; }

		std::unordered_map<String, Cargo> const& get_values() const { return m_values; }

	public:
		static Owner<MaterialTemplate> create(MaterialTemplateBuilder const& builder = {});
	};
}