#pragma once
#include "CS.h"

#include "Minty/Script/ScriptField.h"

namespace Minty
{
	class CsScriptField
		: public ScriptField
	{
	private:
		MonoClassField* mp_field;

	public:
		CsScriptField(ScriptFieldBuilder const& builder, MonoClassField* const field);

		String get_name() const override;

		void set(void* const value) const override;

		void get(void* const value) const override;

		Type get_type() const override;
	};
}