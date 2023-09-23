#pragma once

#include "M_Object.h"
#include "M_EntityRegistry.h"
#include <set>

namespace minty
{
	class Engine;

	/// <summary>
	/// The base class for systems, which provide functionality and conduct the behavior of the ECS engine.
	/// </summary>
	class System
		: public Object
	{
	private:
		// is this system enabled?
		bool _enabled;

	protected:
		/// <summary>
		/// The Engine that this System belongs to.
		/// </summary>
		Engine* _engine;

		/// <summary>
		/// The registry that this System is part of.
		/// </summary>
		EntityRegistry* _registry;

	public:
		/// <summary>
		/// Creates a new System.
		/// </summary>
		System();

		virtual ~System() {}

		/// <summary>
		/// Initializes the System with the Engine and EntityRegistry.
		/// </summary>
		/// <param name="engine"></param>
		/// <param name="registry"></param>
		void init(Engine& engine, EntityRegistry& registry);

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
	};
}