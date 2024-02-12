#pragma once
#include "M_SceneObject.h"

namespace minty
{
	class Runtime;
	class Scene;
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

	public:
		/// <summary>
		/// Creates a new System.
		/// </summary>
		System(Runtime& engine, ID const sceneId);

		virtual ~System();

	public:
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
		virtual void load() {}

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
		virtual void unload() {}
	public:
		friend String to_string(System const& value);
	};
}