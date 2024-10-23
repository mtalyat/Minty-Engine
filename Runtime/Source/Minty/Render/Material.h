#pragma once

#include "Minty/Asset/Asset.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Core/Types.h"
#include "Minty/Core/UUID.h"
#include "Minty/Render/MaterialTemplate.h"
#include <unordered_map>

namespace Minty
{
	struct MaterialBuilder
	{
		UUID id = {};
		Ref<MaterialTemplate> materialTemplate;
		std::unordered_map<String, Cargo> values;
	};

	class Material
		: public Asset
	{
	private:
		Ref<MaterialTemplate> m_materialTemplate;
		std::unordered_map<String, Cargo> m_values;

	public:
		Material(MaterialBuilder const& builder)
			: Asset(builder.id)
			, m_materialTemplate(builder.materialTemplate)
			, m_values(builder.values)
		{}

	public:
		virtual ~Material() = default;

	public:
		AssetType get_type() const override { return AssetType::Material; }

		// binds this Material so it can be drawn with
		void on_bind();

		Ref<MaterialTemplate> get_template() const { return m_materialTemplate; }

	public:
		static Owner<Material> create(const MaterialBuilder& builder = {});
	};
}