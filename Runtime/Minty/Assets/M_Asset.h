#pragma once
#include "Minty/Types/M_Object.h"

#include "Minty/Types/M_Node.h"
#include "Minty/Types/M_Dynamic.h"
#include "Minty/Types/M_UUID.h"

#include <filesystem>
#include <array>
#include <vector>

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
		Text,
		Script,
		AudioClip,
		Animation,
		Animator,
		Texture,
		ShaderCode,
		ShaderModule,
		Shader,
		ShaderPass,
		MaterialTemplate,
		Material,
		Sprite,
		FontVariant,
		Font,
		Mesh,
		Scene,
	};

	String to_string(AssetType const value);
	AssetType from_string_asset_type(String const& value);

	/// <summary>
	/// The base class for all Assets.
	/// </summary>
	class Asset
		: public Object
	{
	private:
		UUID _id;
		Path _path;

	public:
		Asset()
			: _id(UUID::create()), _path() {}

		Asset(UUID const id, Path const& path)
			: _id(id), _path(path) {}

		virtual ~Asset() = default;

		UUID get_id() const { return _id; }

		Path const& get_path() const { return _path; }

		AssetType get_type() const { return Asset::get_type(_path); }

		virtual String get_name() const { return _path.stem().string(); }

		friend bool operator==(Asset const& left, Asset const& right);
		friend bool operator!=(Asset const& left, Asset const& right);

#pragma region Static

		/// <summary>
		/// Gets the meta path of the given asset path.
		/// </summary>
		/// <param name="assetPath"></param>
		/// <returns></returns>
		static Path get_meta_path(Path const& assetPath);

		/// <summary>
		/// Gets the AssetType based on the given path extension.
		/// </summary>
		/// <param name="assetPath"></param>
		/// <returns></returns>
		static AssetType get_type(Path const& assetPath);

		static std::vector<Path> const& get_extensions(AssetType const type);

		/// <summary>
		/// Checks if the given asset type's file is text readable or not.
		/// </summary>
		/// <param name="type"></param>
		/// <returns></returns>
		static bool is_readable(Path const& assetPath);

		/// <summary>
		/// Checks if the given asset type's file is text readable or not.
		/// </summary>
		/// <param name="type"></param>
		/// <returns></returns>
		static bool is_readable(AssetType const type);

		static bool check_type(Path const& path, AssetType const type);

#pragma endregion
	};
}