#pragma once

#include "Minty/Asset/Asset.h"
#include "Minty/Core/Time.h"
#include "Minty/Serialize/Serializable.h"

namespace Minty
{
	class EntityRegistry;
	class SystemRegistry;

	struct SceneBuilder
	{
		UUID id = {};
		std::vector<Path> assets; // registered assets
	};

	/// <summary>
	/// Holds a collection of systems and entities that can interact with one another.
	/// </summary>
	class Scene :
		public Asset, public Source<Scene>, public Serializable
	{
	private:
		struct AssetData
		{
			Size index;
			UUID id;
		};

	private:
		EntityRegistry* mp_entityRegistry;
		SystemRegistry* mp_systemRegistry;
		Bool m_loaded;

		std::unordered_map<Path, AssetData> m_registeredAssets;
		std::vector<Path> m_unloadedAssets;
		std::unordered_set<UUID> m_loadedAssets;

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
		EntityRegistry& get_entity_registry() const { return *mp_entityRegistry; }

		///// <summary>
		///// Gets the SystemRegistry used in the Scene.
		///// </summary>
		///// <returns></returns>
		SystemRegistry& get_system_registry() const { return *mp_systemRegistry; }

		/// <summary>
		/// Checks if this Scene is loaded or not.
		/// </summary>
		/// <returns></returns>
		Bool is_loaded() const { return m_loaded; }

		/// <summary>
		/// Initializes this Scene.
		/// </summary>
		void initialize() override;

		/// <summary>
		/// Loads this Scene.
		/// </summary>
		void load();

		/// <summary>
		/// Updates this Scene.
		/// </summary>
		void update(Time const& time);

		/// <summary>
		/// Sorts the Entities in this Scene based on hierarchy.
		/// </summary>
		void sort();

		/// <summary>
		/// Fixed updates this Scene.
		/// </summary>
		void fixed_update(Time const& time);

		/// <summary>
		/// Unloads this Scene.
		/// </summary>
		void unload();

		/// <summary>
		/// Shuts down this Scene.
		/// </summary>
		void shutdown() override;

		/// <summary>
		/// Finalized updating this Scene.
		/// </summary>
		void finalize();

#pragma region Asset Loading

	public:
		void register_asset(Path const& path);

		void unregister_asset(Path const& path);

		Bool is_registered(Path const& assetPath);

	private:
		void load_registered_assets();

		void unload_registered_assets();

		void update_registered_indices();

#pragma endregion

	public:
		AssetType get_type() const override { return AssetType::Scene; }

		void serialize(Writer& writer) const override;

		void deserialize(Reader& reader) override;

	public:
		static Owner<Scene> create(SceneBuilder const& builder = {});
	};
}