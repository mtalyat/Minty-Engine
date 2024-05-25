#pragma once
#include "Minty/Assets/M_Asset.h"
#include "Minty/Types/M_Time.h"

#include <vector>
#include <unordered_set>
#include <functional>

namespace Minty
{
	class EntityRegistry;
	class SystemRegistry;
	class AssetEngine;

	struct SceneBuilder
	{
		UUID id = UUID::create();

		Path path;
	};

	/// <summary>
	/// Holds a collection of systems and entities that can interact with one another.
	/// </summary>
	class Scene :
		public Asset
	{
	private:
		struct AssetData
		{
			size_t index;
			UUID id;
		};

	private:
		EntityRegistry* _entities;
		SystemRegistry* _systems;
		bool _loaded;

		std::unordered_map<Path, AssetData> _registeredAssets;
		std::vector<Path> _unloadedAssets;
		std::unordered_set<UUID> _loadedAssets;

	public:
		/// <summary>
		/// Creates an empty Scene.
		/// </summary>
		Scene(SceneBuilder const& builder);

		~Scene();

		// move
		Scene(Scene&& other) noexcept;

		// move
		Scene& operator=(Scene&& other) noexcept;

		/// <summary>
		/// Gets the EntityRegistry used in the Scene.
		/// </summary>
		/// <returns></returns>
		EntityRegistry& get_entity_registry() const;

		/// <summary>
		/// Gets the SystemRegistry used in the Scene.
		/// </summary>
		/// <returns></returns>
		SystemRegistry& get_system_registry() const;

		/// <summary>
		/// Checks if this Scene is loaded or not.
		/// </summary>
		/// <returns></returns>
		bool is_loaded() const;

		/// <summary>
		/// Loads this Scene.
		/// </summary>
		void load();

		/// <summary>
		/// Updates this Scene.
		/// </summary>
		void update(Time const time);

		/// <summary>
		/// Sorts the Entities in this Scene based on hierarchy.
		/// </summary>
		void sort();

		/// <summary>
		/// Fixed updates this Scene.
		/// </summary>
		void fixed_update(Time const time);

		/// <summary>
		/// Unloads this Scene.
		/// </summary>
		void unload();

		/// <summary>
		/// Finalized updating this Scene.
		/// </summary>
		void finalize();

#pragma region Asset Loading

	public:
		void register_asset(Path const& path);

		void unregister_asset(Path const& path);

		bool is_registered(Path const& assetPath);

	private:
		void load_registered_assets();

		void unload_registered_assets();

		void sort_registered_assets();

		void update_registered_indices();

#pragma endregion

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

	public:
		friend String to_string(Scene const& value);
	};
}