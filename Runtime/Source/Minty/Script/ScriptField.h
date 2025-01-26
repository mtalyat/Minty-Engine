#pragma once

#include "Minty/Core/Pointer.h"
#include "Minty/Core/Type.h"
#include "Minty/Script/Accessibility.h"

namespace Minty
{
	class ScriptObject;

	struct ScriptFieldBuilder
	{
		Ref<ScriptObject> object;
	};

	class ScriptField
	{
	private:
		// object this field belongs to
		Ref<ScriptObject> m_object;

	protected:
		ScriptField(ScriptFieldBuilder const& builder)
			: m_object(builder.object)
		{}

	public:
		virtual ~ScriptField() {}

	public:
		Ref<ScriptObject> get_object() const { return m_object; }

		virtual String get_name() const = 0;

		virtual void set(void* const value) const = 0;

		virtual void get(void* const value) const = 0;

		virtual Accessibility get_accessibility() const = 0;

		virtual Bool is_static() const = 0;

		virtual Type get_type() const = 0;

		virtual String get_type_full_name() const = 0;
	};
}