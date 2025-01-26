#pragma once
#include "Minty/Component/Component.h"

#include "Minty/Core/UUID.h"

namespace Minty
{
	/// <summary>
	/// Components that have a scripting counterpart.
	/// </summary>
	struct ScriptObjectComponent
		: public Component
	{
	protected:
		/// <summary>
		/// ID to the script object that corresponds with this component.
		/// </summary>
		UUID m_id = INVALID_UUID;

	public:
		virtual void set_id(UUID const id) { m_id = id; }

		virtual UUID get_id() const { return m_id; }

	public:
		virtual ~ScriptObjectComponent() = default;
	};
}