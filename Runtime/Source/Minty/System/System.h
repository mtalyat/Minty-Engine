#pragma once

#include "Minty/Core/Time.h"
#include "Minty/Scene/SceneObject.h"
#include "Minty/Serialize/Serializable.h"
#include <unordered_set>
#include <vector>
#include <functional>

namespace Minty
{
	/// <summary>
	/// The base class for systems, which provide functionality and conduct the behavior of the ECS engine.
	/// </summary>
	class System
		: public SceneObject, public Serializable
	{
	private:
		// Is this System enabled?
		Bool m_enabled;

		// The name of this System.
		String m_name;
	public:
		/// <summary>
		/// Creates a new System.
		/// </summary>
		System(String const& name, Scene& scene)
			: SceneObject(scene), m_enabled(true), m_name(name) {}

		virtual ~System() = default;

	public:
		String const& get_name() const { return m_name; }

	public:
		/// <summary>
		/// Resets the entire system.
		/// </summary>
		virtual void reset() {}

		/// <summary>
		/// Sets the enabled state of this System.
		/// </summary>
		/// <param name="enabled"></param>
		void set_enabled(Bool const enabled) { m_enabled = enabled; }

		/// <summary>
		/// Checks if this System is enabled or not.
		/// </summary>
		/// <returns></returns>
		Bool is_enabled() const { return m_enabled; }

		/// <summary>
		/// Called when the Scene is being loaded.
		/// </summary>
		virtual void load() {}

		/// <summary>
		/// Does one frame of work on the System.
		/// </summary>
		virtual void update(Time const& time) {}

		/// <summary>
		/// Does frame cleanup work on the System.
		/// </summary>
		/// <param name="time"></param>
		virtual void finalize() {}

		/// <summary>
		/// Does frame drawing work on the System.
		/// </summary>
		virtual void draw() {}

		/// <summary>
		/// Does one fixed frame of work on the System.
		/// </summary>
		virtual void fixed_update(Time const time) {}

		/// <summary>
		/// Called when the Scene is being unloaded.
		/// </summary>
		virtual void unload() {}

	public:
		virtual void serialize(Writer& writer) const override {};
		virtual void deserialize(Reader& reader) override {};
	};
}