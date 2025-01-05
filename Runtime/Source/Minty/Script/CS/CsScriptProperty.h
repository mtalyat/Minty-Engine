#pragma once
#include "CS.h"

#include "Minty/Script/ScriptProperty.h"

namespace Minty
{
	class CsScriptProperty
		: public ScriptProperty
	{
	private:
		MonoProperty* mp_property;

	public:
		CsScriptProperty(ScriptPropertyBuilder const& builder, MonoProperty* const property);

		String get_name() const override;

		void set(void** const value) const override;

		void get(void** const value) const override;

		Type get_type() const override;
	};
}