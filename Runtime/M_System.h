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
	private:
		// is this system enabled?
		bool _enabled;

		String _name;
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
	public:
		virtual void serialize(Writer& writer) const override;
		virtual void deserialize(Reader const& reader) override;

	public:
		friend String to_string(System const& value);
	};
}