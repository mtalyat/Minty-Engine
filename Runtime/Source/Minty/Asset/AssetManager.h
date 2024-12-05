#pragma once

#include "Minty/Animation/Animation.h"
#include "Minty/Animation/Animator.h"
#include "Minty/Asset/Asset.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Core/Types.h"
#include "Minty/Core/UUID.h"
#include "Minty/File/Wrapper.h"
#include "Minty/Render/Font.h"
#include "Minty/Render/FontVariant.h"
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
		// determine where to read asset files from
		AssetMode mode = AssetMode::ReadWraps;

		// wrap files to read in
		std::vector<Path> wraps = {};
		
		// should paths be saved in memory, or ignored
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
		// determines how assets are loaded
		static AssetMode s_mode;
		// if true, paths are saved and can be retrieved with get_path()
		static Bool s_savePaths;

		// reference for all assets, by ID
		static std::unordered_map<UUID, AssetData> s_assets;
		// reference for all assets, by type
		static std::unordered_map<AssetType, std::unordered_set<Ref<Asset>>> s_assetsByType;

		// holds all loaded .wrap data
		static Wrapper s_wrapper;

	private:
		AssetManager() = default;
		~AssetManager() = default;
		
#pragma region Core

	public:
		/// <summary>
		/// Initializes the AssetManager.
		/// </summary>
		/// <param name="builder"></param>
		static void initialize(AssetManagerBuilder const& builder);

		/// <summary>
		/// Shuts down the AssetManager.
		/// </summary>
		static void shutdown();

#pragma endregion

#pragma region Files

	private:
		// opens a file at the given path for reading
		static File* open(Path const& path);

	public:
		/// <summary>
		/// Opens a file and reader at the given path.
		/// </summary>
		/// <param name="path">The path to the file to open.</param>
		/// <param name="container">The container to read the data from.</param>
		/// <param name="reader">The reader to use to read the data.</param>
		/// <returns></returns>
		static Bool open_reader(Path const& path, Container*& container, Reader*& reader);

		/// <summary>
		/// Deletes all file and reader resources.
		/// </summary>
		/// <param name="container"></param>
		/// <param name="reader"></param>
		static void close_reader(Container*& container, Reader*& reader);

#pragma endregion

	public:
		inline static Wrapper& get_wrapper() { return s_wrapper; }

		/// <summary>
		// Reads the ID from the meta file of the given Asset Path.
		/// </summary>
		/// <param name="path">The Path to the Asset file.</param>
		/// <returns>The ID of the Asset.</returns>
		static UUID read_id(Path const& path);

		/// <summary>
		/// Writes the ID to a meta file to the given Asset Path.
		/// </summary>
		/// <param name="path">The Path of the Asset file.</param>
		/// <param name="id">The ID to write.</param>
		static Bool write_id(Path const& path, UUID const id);

		/// <summary>
		/// Checks if an Asset with the given Path exists.
		/// </summary>
		/// <param name="path">The path to the Asset file.</param>
		/// <returns>True if the Path exists.</returns>
		static Bool exists(Path const& path);

		/// <summary>
		/// Loads an Asset from the disc.
		/// </summary>
		/// <param name="path">The path to the Asset file.</param>
		/// <returns>The newly loaded Asset.</returns>
		static Ref<Asset> load_asset(Path const& path);

		/// <summary>
		/// Loads a type of Asset from the disc.
		/// </summary>
		/// <typeparam name="T">The type to load.</typeparam>
		/// <param name="path">The path to the Asset file.</param>
		/// <returns>The newly loaded Asset.</returns>
		template<typename T>
		static Ref<T> load(Path const& path)
		{
			return static_cast<Ref<T>>(load_asset(path));
		}

		/// <summary>
		/// Saves an Asset to the disc.
		/// </summary>
		/// <param name="path">The path to the Asset file.</param>
		/// <param name="asset">The Asset to save.</param>
		static Bool save_asset(Path const& path, Ref<Asset> const& asset);

		/// <summary>
		/// Saves an Asset to the disc.
		/// </summary>
		/// <typeparam name="T">The type of Asset.</typeparam>
		/// <param name="path">The path to the Asset file.</param>
		/// <param name="asset">The Asset to save.</param>
		template<typename T>
		static Bool save(Path const& path, Ref<T> const& asset)
		{
			return save_asset(path, asset);
		}
		
		/// <summary>
		/// Creates an empty asset of the given type.
		/// </summary>
		/// <typeparam name="T">The type to create.</typeparam>
		/// <returns>The newly created Asset.</returns>
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
		/// <typeparam name="T">The type to create.</typeparam>
		/// <typeparam name="...Args">The types of arguments to pass into the create function for the given type.</typeparam>
		/// <param name="...args">The arguments to pass into the create function for the given type.</param>
		/// <returns>The newly created Asset.</returns>
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
		/// <typeparam name="T">The type to create.</typeparam>
		/// <typeparam name="...Args">The types of arguments to pass into the create function for the given type.</typeparam>
		/// <param name="path">The path to the Asset.</param>
		/// <param name="...args">The arguments to pass into the create function for the given type.</param>
		/// <returns>The newly created Asset.</returns>
		template<typename T, typename... Args>
		static Ref<T> create_existing(Path const& path, Args&&... args)
		{
			Owner<T> asset = T::create(std::forward<Args>(args)...);
			emplace(path, asset);
			return asset;
		}

		/// <summary>
		/// Adds the asset to its corresponding type list.
		/// </summary>
		/// <param name="asset"></param>
		static void emplace_by_type(Ref<Asset> const asset);

		/// <summary>
		/// Removes the asset from its corresponding type list.
		/// </summary>
		/// <param name="asset"></param>
		static void erase_by_type(Ref<Asset> const asset);

		/// <summary>
		/// Checks for a dependency, and gets a ref to it if able.
		/// </summary>
		/// <typeparam name="T">The type of Asset to find.</typeparam>
		/// <param name="path">The path to the file that is being worked on.</param>
		/// <param name="reader">The reader to the file that is being worked on.</param>
		/// <param name="name">The name of the data to load from the Reader.</param>
		/// <param name="asset">A reference to the Asset to load the data into.</param>
		/// <param name="required">If true, an error is raised if no valid value with the given name is found.</param>
		/// <returns>True if found and stored into asset.</returns>
		template<typename T>
		static Bool find_dependency(Path const& path, Reader& reader, String const& name, Ref<T>& asset, bool required)
		{
			UUID id{};

			// if nothing read, set to null
			if (!reader.read(name, id))
			{
				if (required)
				{
					Debug::log_error(std::format("Cannot load \"{}\". Missing \"{}\".", path.generic_string(), name));
				}

				asset.release();
				return false;
			}

			// something read
			
			// if invalid id (0), set to null
			if (!id.valid())
			{
				if (required)
				{
					Debug::log_error(std::format("Cannot load \"{}\". \"{}\"'s ID was invalid.", path.generic_string(), name));
				}

				asset.release();
				return false;
			}

			// if asset id is valid but asset with id DNE, set to null
			if (!id.valid() || !contains(id))
			{
				Debug::log_error(std::format("Cannot load \"{}\". \"{}\" requires a dependency that has not been loaded yet, with ID {}.", path.generic_string(), name, to_string(id)));
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
		/// <param name="id">The ID of the asset.</param>
		static void unload(UUID const id);

		/// <summary>
		/// Unloads all Assets from this AssetManager.
		/// </summary>
		static void unload_all();
		
		/// <summary>
		/// Adds an Asset into this AssetManager.
		/// </summary>
		/// <param name="path">The path of the Asset.</param>
		/// <param name="asset">The Asset to add.</param>
		static void emplace(Path const& path, Owner<Asset> const asset);

		/// <summary>
		/// Removes an Asset from this AssetManager.
		/// </summary>
		/// <param name="id">The ID of the Asset to erase.</param>
		/// <returns>True if successfully erased an Asset.</returns>
		static Bool erase(UUID const id);

		/// <summary>
		/// Gets the Asset with the given ID, if able.
		/// </summary>
		/// <param name="id">The ID of the Asset to get.</param>
		/// <returns>A reference to the Asset with the correspondng ID.</returns>
		static Ref<Asset> get_asset(UUID const id);

		/// <summary>
		/// Gets the Asset with the given ID, if able.
		/// </summary>
		/// <typeparam name="T">The type of Asset to get.</typeparam>
		/// <param name="id">The ID of the Asset to get.</param>
		/// <returns>A reference to the Asset with the correspondng ID.</returns>
		template<typename T>
		static Ref<T> get(UUID const id)
		{
			return static_cast<Ref<T>>(get_asset(id));
		}

		/// <summary>
		/// Gets a list of Assets with the given type.
		/// </summary>
		/// <typeparam name="T">The type of Assets to get.</typeparam>
		/// <returns>A list of references to Assets with the type.</returns>
		template<typename T>
		static std::vector<Ref<T>> get_by_type()
		{
			TypeID typeId = typeid(T);
			AssetType type = asset_type_typeid(typeId);

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

		/// <summary>
		/// Gets the corresponding Path to the Asset with the given ID.
		/// </summary>
		/// <param name="id">The ID of the Asset.</param>
		/// <returns>The Path to the Asset, or an empty Path if no Path found.</returns>
		static Path get_path(UUID const id);

		/// <summary>
		/// Gets the file name of the corresponding Path to the Asset with the given ID.
		/// </summary>
		/// <param name="id">The ID of the Asset.</param>
		/// <returns>The file name of the Asset without the extension.</returns>
		static String get_name(UUID const id);

		/// <summary>
		/// Checks if an Asset with the given ID exists within this AssetManager.
		/// </summary>
		/// <param name="id">The ID of the Asset.</param>
		/// <returns>True if the Asset exists.</returns>
		static Bool contains(UUID const id);

		/// <summary>
		/// Clones the Asset with the given ID.
		/// </summary>
		/// <typeparam name="T">The type of Asset to clone.</typeparam>
		/// <param name="id">The ID of the existing Asset.</param>
		/// <returns>A reference to the newly cloned Asset.</returns>
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

		/// <summary>
		/// Checks if the given Asset has any Assets that depend on it.
		/// </summary>
		/// <param name="asset"></param>
		/// <returns></returns>
		static std::vector<Ref<Asset>> get_dependents(Ref<Asset> const asset);

#pragma region Reading

	public:
		/// <summary>
		/// Reads all of the Text from the file at the given Path.
		/// </summary>
		/// <param name="path">The Path to the file.</param>
		/// <returns>The file contents.</returns>
		static String read_file(Path const& path);

		/// <summary>
		/// Reads the raw characters from the file at the given Path.
		/// </summary>
		/// <param name="path">The Path to the file.</param>
		/// <returns>The file contents.</returns>
		static std::vector<Char> read_file_chars(Path const& path);

		/// <summary>
		/// Reads the raw bytes from the file at the given Path.
		/// </summary>
		/// <param name="path">The Path to the file.</param>
		/// <returns>The file contents.</returns>
		static std::vector<Byte> read_file_bytes(Path const& path);

		/// <summary>
		/// Reads the lines from the file at the given Path.
		/// </summary>
		/// <param name="path">The Path to the file.</param>
		/// <returns>The file contents.</returns>
		static std::vector<String> read_file_lines(Path const& path);

#pragma endregion

#pragma region Load Methods

	private:
		static Ref<Animation> load_animation(Path const& path);

		static Ref<Animator> load_animator(Path const& path);

		static Ref<Font> load_font(Path const& path);

		static Ref<FontVariant> load_font_variant(Path const& path);

		static Owner<Image> load_image(Path const& path);

		static Bool load_values(Reader& reader, std::unordered_map<String, Cargo>& values, Ref<Shader> const shader, Path const& path);

		static Ref<Material> load_material(Path const& path);

		static Ref<MaterialTemplate> load_material_template(Path const& path);

		static Ref<Scene> load_scene(Path const& path);

		static Ref<Shader> load_shader(Path const& path);

		static Ref<ShaderModule> load_shader_module(Path const& path);

		static Ref<Sprite> load_sprite(Path const& path);

		static Ref<Texture> load_texture(Path const& path);\

#pragma endregion

#pragma region Save Methods

	private:
		static Bool save_scene(Path const& path, Ref<Scene> const& scene);

#pragma endregion
	};
}