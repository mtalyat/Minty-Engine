#pragma once

#include "Minty/Core/Pointer.h"
#include "Minty/Core/Type.h"

namespace Minty
{
	class ScriptObject;

	struct ScriptPropertyBuilder
	{
		Ref<ScriptObject> object;
	};

	class ScriptProperty
	{
	private:
		Ref<ScriptObject> m_object;

	protected:
		ScriptProperty(ScriptPropertyBuilder const& builder)
			: m_object(builder.object)
		{}

	public:
		virtual ~ScriptProperty() {}

	public:
		Ref<ScriptObject> get_object() const { return m_object; }

		virtual String get_name() const = 0;

		virtual void set(void** const value) const = 0;

		virtual void get(void** const value) const = 0;

		virtual Type get_type() const = 0;
	};
}