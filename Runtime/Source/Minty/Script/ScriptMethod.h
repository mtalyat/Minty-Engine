#pragma once

#include "Minty/Core/Pointer.h"
#include "Minty/Core/Type.h"
#include "Minty/Script/Accessibility.h"

namespace Minty
{
	class ScriptObject;

	struct ScriptMethodBuilder
	{
		Ref<ScriptObject> object;
	};

	class ScriptMethod
	{
	private:
		Ref<ScriptObject> m_object;

	protected:
		ScriptMethod(ScriptMethodBuilder const& builder)
			: m_object(builder.object)
		{}

	public:
		virtual ~ScriptMethod() {}

	public:
		Ref<ScriptObject> get_object() const { return m_object; }

		virtual String get_name() const = 0;

		virtual Accessibility get_accessibility() const = 0;

		virtual Bool is_static() const = 0;

		/// <summary>
		/// Invokes the method with no arguments and no return value.
		/// </summary>
		virtual void invoke() const = 0;

		/// <summary>
		/// Invokes the method with the given arguments and no return values.
		/// </summary>
		/// <param name="argv"></param>
		/// <param name="argc"></param>
		virtual void invoke(void** const argv, Size const argc) const = 0;
	};
}