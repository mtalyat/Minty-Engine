#pragma once
#include "Minty/Scenes/M_SceneObject.h"
#include "Minty/Types/M_Time.h"

#include <unordered_set>
#include <vector>
#include <functional>

namespace Minty
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
	private:
		// is this system enabled?
		bool _enabled;

		String _name;
	public:
		/// <summary>
		/// Creates a new System.
		/// </summary>
		System(String const& name, Scene& scene)
			: SceneObject(scene), _enabled(true), _name(name) {}

		virtual ~System() = default;

	public:
		String const& get_name() const { return _name; }

		EntityRegistry& get_entity_registry() const;

		SystemRegistry& get_system_registry() const;

	public:
		/// <summary>
		/// Resets the entire system.
		/// </summary>
		virtual void reset() {}

		/// <summary>
		/// Sets the enabled state of this System.
		/// </summary>
		/// <param name="enabled"></param>
		void set_enabled(bool const enabled) { _enabled = enabled; }

		/// <summary>
		/// Checks if this System is enabled or not.
		/// </summary>
		/// <returns></returns>
		bool is_enabled() const { return _enabled; }

		/// <summary>
		/// Called when the Scene is being loaded.
		/// </summary>
		virtual void load() {}

		/// <summary>
		/// Does one frame of work on the System.
		/// </summary>
		virtual void update(Time const time) {}

		/// <summary>
		/// Does one fixed frame of work on the System.
		/// </summary>
		virtual void fixed_update(Time const time) {}

		/// <summary>
		/// Called when the Scene is being unloaded.
		/// </summary>
		virtual void unload() {}
	};
}