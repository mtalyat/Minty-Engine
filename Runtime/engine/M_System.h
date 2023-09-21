#pragma once

#include "M_Object.h"
#include "M_Entities.h"
#include <set>

namespace minty
{
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
		/// The registry that this System is part of.
		/// </summary>
		Registry& _registry;

	public:
		/// <summary>
		/// Creates a new System.
		/// </summary>
		/// <param name="registry">The entity registry belonging to this System.</param>
		System(Registry& registry);

		virtual ~System() {}

		/// <summary>
		/// Enables this System. The System will run update() or fixedUpdate() when enabled.
		/// </summary>
		void enable();

		/// <summary>
		/// Disables this System. The System will not run update() or fixedUpdate() when disabled.
		/// </summary>
		void disable();

		/// <summary>
		/// Sets the enabled state of this System.
		/// </summary>
		/// <param name="enabled"></param>
		void setEnabled(bool const enabled);

		/// <summary>
		/// Checks if this System is enabled or not.
		/// </summary>
		/// <returns></returns>
		constexpr bool isEnabled() const;

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
		virtual void fixedUpdate() {}

		/// <summary>
		/// Called when the Scene is being unloaded.
		/// </summary>
		virtual void unload() {}
	};
}