#pragma once

#include "Minty/Asset/Asset.h"
#include "Minty/Core/Path.h"

namespace Minty
{
	struct ShaderModuleBuilder
	{
		UUID id;
		Path path;
	};

	/// <summary>
	/// Holds data for a shader module (shader code) that can be used in the RendererInterface.
	/// </summary>
	class ShaderModule
		: public Asset
	{
	public:
		ShaderModule(const ShaderModuleBuilder& builder)
			: Asset::Asset(builder.id)
		{}

		virtual ~ShaderModule() = default;

		AssetType get_type() const override { return AssetType::ShaderModule; }

		virtual void* get_native() const = 0;

		static Owner<ShaderModule> create(const ShaderModuleBuilder& builder = {});
	};
}