#pragma once

#include "Minty/Animation/Animation.h"
#include "Minty/Animation/Animator.h"
#include "Minty/Asset/Asset.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Core/Types.h"
#include "Minty/Core/UUID.h"
#include "Minty/File/Wrapper.h"
#include "Minty/Render/Image.h"
#include "Minty/Render/Material.h"
#include "Minty/Render/MaterialTemplate.h"
#include "Minty/Render/ShaderModule.h"
#include "Minty/Render/Texture.h"
#include "Minty/Render/Sprite.h"
#include "Minty/Scene/Scene.h"
#include "Minty/Serialize/Reader.h"
#include "Minty/Serialize/Writer.h"
#include <unordered_map>
#include <vector>

namespace Minty
{
	/// <summary>
	/// Controls how Assets are loaded.
	/// </summary>
	enum class AssetMode
	{
		// reads from file system
		ReadFiles = 0b0001,
		// reads from wrap files
		ReadWraps = 0b0010,
		// reads from file system, or wrapper if dne on file system
		ReadAll = 0b0011,
	};

	struct AssetManagerBuilder
	{
		AssetMode mode = AssetMode::ReadWraps;
		std::vector<Path> wraps = {};
		Bool savePaths = false;
	};

	/// <summary>
	/// Handles the loading and unloading of assets.
	/// </summary>
	class AssetManager
	{
	private:
		struct AssetData
		{
			Owner<Asset> asset;
			Path path;
		};

	private:
		static AssetMode s_mode;
		static Bool s_savePaths;

		static std::unordered_map<UUID, AssetData> s_assets;
		static std::unordered_map<AssetType, std::unordered_set<Ref<Asset>>> s_assetsByType;

		static Wrapper s_wrapper;

	private:
		AssetManager() = default;
		~AssetManager() = default;

	public:
		static void initialize(const AssetManagerBuilder& builder);
		static void shutdown();

	private:
		// opens a file at the given path for reading
		static File* open(const Path& path);

	public:
		// opens a file and reader at the given path
		static Bool open_reader(const Path& path, Container*& container, Reader*& reader);

		// deletes all file and reader resources
		static void close_reader(Container*& container, Reader*& reader);

	public:
		// reads the ID from the meta file of the given asset path
		static UUID read_id(const Path& path);

		/// <summary>
		/// Checks if an Asset with the given Path exists.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		static Bool exists(const Path& path);

		/// <summary>
		/// Loads an Asset from the disc.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		static Ref<Asset> load_asset(const Path& path);

		/// <summary>
		/// Loads a type of Asset from the disc.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="path"></param>
		/// <returns></returns>
		template<typename T>
		static Ref<T> load(const Path& path)
		{
			return static_cast<Ref<T>>(load_asset(path));
		}

		// creates an empty asset of the given type
		template<typename T>
		static Ref<T> create()
		{
			// create asset
			Owner<T> asset = T::create();
			emplace("", asset);
			return asset;
		}

		/// <summary>
		/// Creates a new Asset.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <typeparam name="...Args"></typeparam>
		/// <param name="...args"></param>
		/// <returns></returns>
		template<typename T, typename... Args>
		static Ref<T> create(Args&&... args)
		{
			Owner<T> asset = T::create(std::forward<Args>(args)...);
			emplace("", asset);
			return asset;
		}

	private:
		/// <summary>
		/// Creates an existing asset that is associated with a file path.
		/// </summary>
		/// <typeparam name="...Args"></typeparam>
		/// <typeparam name="T"></typeparam>
		/// <param name="path"></param>
		/// <param name="...args"></param>
		/// <returns></returns>
		template<typename T, typename... Args>
		static Ref<T> create_existing(const Path& path, Args&&... args)
		{
			Owner<T> asset = T::create(std::forward<Args>(args)...);
			emplace(path, asset);
			return asset;
		}

		static void emplace_by_type(Ref<Asset> const asset);

		static void erase_by_type(Ref<Asset> const asset);

		template<typename T>
		static Bool find_dependency(Path const& path, Reader& reader, String const& name, Ref<T>& asset)
		{
			UUID id{};

			// if nothing read, set to null
			if (!reader.read(name, id))
			{
				asset.release();
				return false;
			}

			// something read
			
			// if invalid id (0), set to null
			if (!id.valid())
			{
				asset.release();
				return false;
			}

			// if asset id is valid but asset with id DNE, set to null
			if (!check_dependency(path, name, id))
			{
				asset.release();
				return false;
			}

			// load asset
			asset = get<T>(id);
			return true;
		}

	public:
		/// <summary>
		/// Unloads the Asset with the given ID.
		/// </summary>
		/// <param name="id"></param>
		static void unload(const UUID id);

		/// <summary>
		/// Unloads all Assets from this AssetManager.
		/// </summary>
		static void unload_all();

		/// <summary>
		/// Adds an Asset into this AssetManager.
		/// </summary>
		/// <param name="asset"></param>
		static void emplace(const Path& path, const Owner<Asset> asset);

		/// <summary>
		/// Removes an Asset from this AssetManager.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		static Bool erase(const UUID id);

		/// <summary>
		/// Gets the Asset with the given ID, if able.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		static Ref<Asset> get_asset(UUID const id);

		template<typename T>
		static Ref<T> get(UUID const id)
		{
			return static_cast<Ref<T>>(get_asset(id));
		}

		static Ref<Asset> at_asset(UUID const id);

		template<typename T>
		static Ref<T> at(UUID const id)
		{
			return static_cast<Ref<T>>(at_asset(id));
		}

		template<typename T>
		static std::vector<Ref<T>> get_by_type(AssetType const type)
		{
			auto found = s_assetsByType.find(type);

			if (found == s_assetsByType.end())
			{
				return {};
			}

			std::vector<Ref<T>> output;
			output.reserve(found->second.size());

			for (auto const& asset : found->second)
			{
				output.push_back(static_cast<Ref<T>>(asset));
			}

			return output;
		}

		static Path get_path(UUID const id);

		/// <summary>
		/// Checks if an Asset with the given ID exists within this AssetManager.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		static Bool contains(const UUID id);

		template<typename T>
		static Ref<T> clone(UUID const id)
		{
			// get asset
			Ref<T> asset = get<T>(id);

			// do nothing if null
			if (asset == nullptr)
			{
				return Ref<T>();
			}

			// create new asset of same type
			Ref<T> newAsset = create<T>();

			// copy the value
			*newAsset.get() = *asset.get();

			// return new cloned asset
			return newAsset;
		}
#pragma region Reading

	public:
		static String read_file(Path const& path);

		static std::vector<Char> read_file_chars(Path const& path);

		static std::vector<Byte> read_file_bytes(Path const& path);

		static std::vector<String> read_file_lines(Path const& path);

#pragma endregion

	private:
		// checks for a dependency that another asset has while loading
		static Bool check_dependency(Path const& path, String const& name, const UUID id);

		static void missing_dependency(Path const& path, String const& name, UUID const id);

		static Ref<Animation> load_animation(const Path& path);

		static Ref<Animator> load_animator(const Path& path);

		static Owner<Image> load_image(const Path& path);

		static Bool load_values(Reader& reader, std::unordered_map<String, Cargo>& values, Ref<Shader> const shader, Path const& path);

		static Ref<Material> load_material(Path const& path);

		static Ref<MaterialTemplate> load_material_template(Path const& path);

		static Ref<Scene> load_scene(Path const& path);

		static Ref<Shader> load_shader(const Path& path);

		static Ref<ShaderModule> load_shader_module(const Path& path);

		static Ref<Sprite> load_sprite(Path const& path);

		static Ref<Texture> load_texture(const Path& path);
	};
}