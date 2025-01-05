#pragma once

#include "Minty/Core/Types.h"
#include "Minty/Core/UUID.h"

namespace Minty
{
	/// <summary>
	/// Types of Assets that can be loaded into the engine.
	/// 
	/// This determines the loading order of the Assets when loading the Scene.
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

	AssetType asset_type_typeid(TypeID const& typeId);

	/// <summary>
	/// The base class for all Assets.
	/// </summary>
	class Asset
	{
		friend class AssetManager;

	private:
		UUID m_id;

	public:
		Asset(UUID const id)
			: m_id(id ? id : UUID::create()) // set ID if given ID is zero
		{}

		virtual ~Asset() = default;

		/// <summary>
		/// Performs initialization when the Asset is created.
		/// </summary>
		virtual void initialize() {}

		/// <summary>
		/// Performs shutdown when the Asset is destroyed.
		/// </summary>
		virtual void shutdown() {}

		/// <summary>
		/// Gets the ID of this Asset.
		/// </summary>
		/// <returns></returns>
		UUID id() const { return m_id; }

		/// <summary>
		/// Gets the AssetType that corresponds to this Asset.
		/// </summary>
		/// <returns></returns>
		virtual AssetType get_type() const = 0;

		/// <summary>
		/// Gets the AssetType based on the extension in the given Path.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		static AssetType get_type_from_path(Path const& path);

		/// <summary>
		/// Gets the meta file Path for the given Asset Path.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		static Path get_meta_path(Path const& path);

		/// <summary>
		/// Gets a list of extensions for the given type.
		/// </summary>
		/// <param name="assetType"></param>
		/// <returns></returns>
		static std::vector<Path> get_extensions(AssetType const type);
	};
}