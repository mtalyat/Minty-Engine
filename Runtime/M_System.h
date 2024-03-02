#pragma once
#include "M_SceneObject.h"

#include <unordered_set>
#include <vector>
#include <functional>

namespace minty
{
	class Asset;
	class AssetEngine;
	class EntityRegistry;
	class SystemRegistry;

	/// <summary>
	/// The base class for systems, which provide functionality and conduct the behavior of the ECS engine.
	/// </summary>
	class System
		: public SceneObject
	{
	protected:
		typedef std::function<Asset*(AssetEngine& engine, Path const&)> RegisterFunc;

	private:
		// is this system enabled?
		bool _enabled;

		String _name;

		std::vector<std::pair<std::vector<Path>, RegisterFunc>> _unloadedAssets;
		std::unordered_set<UUID> _loadedAssets;
	public:
		/// <summary>
		/// Creates a new System.
		/// </summary>
		System(String const& name, Runtime& runtime, Scene& scene);

		virtual ~System();

	public:
		String const& get_name() const;

		EntityRegistry& get_entity_registry() const;

		SystemRegistry& get_system_registry() const;

	public:
		/// <summary>
		/// Resets the entire system.
		/// </summary>
		virtual void reset();

		/// <summary>
		/// Sets the enabled state of this System.
		/// </summary>
		/// <param name="enabled"></param>
		void set_enabled(bool const enabled);

		/// <summary>
		/// Checks if this System is enabled or not.
		/// </summary>
		/// <returns></returns>
		bool is_enabled() const;

		/// <summary>
		/// Called when the Scene is being loaded.
		/// </summary>
		virtual void load();

		/// <summary>
		/// Does one frame of work on the System.
		/// </summary>
		virtual void update() {}

		/// <summary>
		/// Does one fixed frame of work on the System.
		/// </summary>
		virtual void fixed_update() {}

		/// <summary>
		/// Called when the Scene is being unloaded.
		/// </summary>
		virtual void unload();

	protected:
		void register_assets(std::vector<Path> const& paths, RegisterFunc const& func);

		void register_assets(Reader const& reader, String const& name, RegisterFunc const& func);

		void load_registered_assets();

		void unload_registered_assets();

	public:
		friend String to_string(System const& value);
	};
}