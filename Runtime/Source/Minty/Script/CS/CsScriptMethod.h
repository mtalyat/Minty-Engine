#pragma once
#include "CS.h"

#include "Minty/Script/ScriptMethod.h"

namespace Minty
{
	class CsScriptMethod
		: public ScriptMethod
	{
	private:
		MonoMethod* mp_method;

	public:
		CsScriptMethod(ScriptMethodBuilder const& builder, MonoMethod* const method);

		String get_name() const override;

		Accessibility get_accessibility() const override;

		Bool is_static() const override;

		void invoke() const override;

		void invoke(void** const argv, Size const argc) const override;
	};
}