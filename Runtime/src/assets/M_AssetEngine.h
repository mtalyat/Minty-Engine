#pragma once
#include "runtime/M_Engine.h"

#include "types/M_Types.h"
#include "assets/M_Asset.h"
#include "files/M_Wrapper.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace minty
{
	class File;

	class Texture;
	class Sprite;
	class Shader;
	class ShaderPass;
	class MaterialTemplate;
	class Material;
	class Mesh;

	class AudioClip;

	class Animation;
	class Animator;

	class AssetEngine
		: public Engine
	{
	private:
		std::unordered_map<UUID, Asset*> _assets;
		std::unordered_map<TypeID, std::unordered_set<Asset*>> _assetsByType;

		Wrapper _wrapper;

	public:
		AssetEngine(Runtime& runtime);

		virtual ~AssetEngine();

#pragma region Loading

	public:
		bool exists(Path const& path) const;

		std::vector<char> read_file(Path const& path) const;

		String read_text(Path const& path) const;

		// reads the file at the given path as a node
		Node read_file_node(Path const& path) const;

		// reads the file at the given path + .mmeta as a node
		Node read_file_meta(Path const& path) const;

		std::vector<Byte> read_file_bytes(Path const& path) const;

		std::vector<String> read_file_lines(Path const& path) const;

		/// <summary>
		/// Loads a generic Asset.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		Asset* load_asset(Path const& path);

		/// <summary>
		/// Loads just the UUID from the asset at the given path.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		UUID read_id(Path const& path);

		/// <summary>
		/// Loads the asset of the given type from the path.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="path"></param>
		/// <returns></returns>
		template<typename T>
		T* load(Path const& path);

		Wrapper& get_wrapper();

		Wrapper const& get_wrapper() const;

	private:
		bool check(Path const& path) const;

#pragma region Loading Types

	private:
		int check_dependencies(std::vector<void*> const& dependencies) const;

	private:
		/// <summary>
		/// Loads the Texture at the given Path and returns its ID.
		/// </summary>
		/// <param name="path">The path to the asset file.</param>
		/// <returns>The UUID of the Texture, or 0 if the asset does not exist.</returns>
		Texture* load_texture(Path const& path);

		/// <summary>
		/// Loads the Sprite at the given Path and returns its ID.
		/// </summary>
		/// <param name="path">The path to the asset file.</param>
		/// <returns>The UUID of the Texture, or 0 if the asset does not exist.</returns>
		Sprite* load_sprite(Path const& path);

		Shader* load_shader(Path const& path);

		ShaderPass* load_shader_pass(Path const& path);

		MaterialTemplate* load_material_template(Path const& path);

		Material* load_material(Path const& path);

		Mesh* load_mesh(Path const& path);

		void load_mesh_obj(Path const& path, Mesh& mesh);

		AudioClip* load_audio_clip(Path const& path);

		Animation* load_animation(Path const& path);

		Animator* load_animator(Path const& path);

		Asset* load_script(Path const& path);

		Asset* load_text(Path const& path);

#pragma endregion

#pragma endregion

	public:
		/// <summary>
		/// Unloads the asset with the given ID.
		/// </summary>
		/// <param name="id"></param>
		void unload(UUID const id);

		/// <summary>
		/// Unloads the given asset.
		/// </summary>
		/// <param name="asset"></param>
		/// <returns></returns>
		void unload(Asset const& asset);

		/// <summary>
		/// Unloads all assets in this AssetManager.
		/// </summary>
		void unload_all();

		/// <summary>
		/// Checks if the given asset has any dependents.
		/// </summary>
		/// <param name="asset"></param>
		/// <returns>True if any other assets need the given asset in order to function.</returns>
		std::vector<Asset*> get_dependents(Asset const& asset) const;

		/// <summary>
		/// Gets the asset with the given ID.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="id"></param>
		/// <returns></returns>
		template<typename T>
		T& at(UUID const id) const
		{
			MINTY_ASSERT_FORMAT(_assets.contains(id), "{}", to_string(id));

			return *static_cast<T*>(_assets.at(id));
		}

		/// <summary>
		/// Tries to get the asset with the given ID.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="id"></param>
		/// <returns>A pointer to the asset, or nullptr if no asset with the given ID exists.</returns>
		template<typename T>
		T* get(UUID const id) const
		{
			auto found = _assets.find(id);

			if (found == _assets.end())
			{
				return nullptr;
			}

			return static_cast<T*>(found->second);
		}

		Asset* get_asset(UUID const id) const;

		bool contains(UUID const id) const;

		/// <summary>
		/// Adds the given asset to this AssetEngine.
		/// </summary>
		/// <param name="asset"></param>
		/// <returns></returns>
		void emplace(Asset* const asset);

		/// <summary>
		/// Removes an asset from this AssetEngine. Does not delete the asset.
		/// </summary>
		/// <param name="id"></param>
		void erase(UUID const id);

		template<class T>
		T* emplace_new(T* const asset)
		{
			emplace(asset);
			return asset;
		}

		template<class T>
		std::vector<T*> get_by_type() const
		{
			std::vector<T*> result;

			TypeID typeId = typeid(T);

			auto found = _assetsByType.find(typeId);

			if (found != _assetsByType.end())
			{
				// type exists, add all to result
				result.reserve(found->second.size());

				for (auto const asset : found->second)
				{
					result.push_back(static_cast<T*>(asset));
				}
			}

			// no type, return empty
			return result;
		}

	private:
		void emplace_by_type(Asset& asset);

		void erase_by_type(Asset& asset);
	};

	template<typename T>
	T* AssetEngine::load(Path const& path)
	{
		return static_cast<T*>(load_asset(path));
	}
}