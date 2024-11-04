#pragma once

#include "Minty/Core/Types.h"
#include "Minty/Core/UUID.h"

namespace Minty
{
	/// <summary>
	/// All of the types of assets that can be loaded into the engine.
	/// 
	/// This determines the loading order of the assets when loading the scene.
	/// </summary>
	enum class AssetType
	{
		None,
		Meta,
		Wrap,
		Info, // application info
		Text,
		Script,
		AudioClip,
		Animation,
		Animator,
		Viewport,
		Scissor,
		Texture,
		ShaderCode,
		ShaderModule,
		Shader,
		//ShaderPass,
		MaterialTemplate,
		Material,
		Sprite,
		FontVariant,
		Font,
		Mesh,
		Scene,
	};

	/// <summary>
	/// The base class for all Assets.
	/// </summary>
	class Asset
	{
		friend class AssetManager;

	private:
		UUID m_id;

	public:
		Asset(const UUID id)
			: m_id(id ? id : UUID::create()) // set ID if given ID is zero
		{}

		virtual ~Asset() = default;

		virtual void initialize() {}

		virtual void shutdown() {}

		UUID id() const { return m_id; }

		virtual AssetType get_type() const = 0;

		static AssetType get_type_from_path(const Path& path);

		static Path get_meta_path(const Path& path);
	};
}