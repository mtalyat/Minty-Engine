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
		Material(MaterialBuilder const& builder);

	public:
		virtual ~Material();

	public:
		AssetType get_asset_type() const override { return AssetType::Material; }

		// binds this Material so it can be drawn with
		virtual void on_bind() = 0;

		Ref<MaterialTemplate> get_template() const { return m_materialTemplate; }

		std::unordered_map<String, Cargo> const& get_values() const { return m_values; }

		virtual void set_input(String const& name, void const* const data) = 0;

		Bool try_set_input(String const& name, void const* const data);

	public:
		static Owner<Material> create(MaterialBuilder const& builder = {});
	};
}