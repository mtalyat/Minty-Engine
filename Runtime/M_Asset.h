#pragma once
#include "M_RuntimeObject.h"

#include "M_Node.h"
#include "M_Dynamic.h"
#include "M_UUID.h"

#include <filesystem>
#include <array>
#include <vector>

namespace minty
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
		Mesh,
		Scene,
	};

	class Asset
		: public RuntimeObject
	{
	private:
		UUID _id;
		Path _path;

	public:
		Asset();

		Asset(UUID const id, Path const& path, Runtime& runtime);

		virtual ~Asset();

		UUID get_id() const;

		Path const& get_path() const;

		AssetType get_type() const;

		virtual String get_name() const;

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