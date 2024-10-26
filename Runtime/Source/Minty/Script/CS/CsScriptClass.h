#pragma once
#include "CS.h"

#include "Minty/Script/ScriptClass.h"
#include <unordered_map>

namespace Minty
{
	class CsScriptClass
		: public ScriptClass
	{
	private:
		MonoClass* mp_class;

	public:
		CsScriptClass(ScriptClassBuilder const& builder);

		~CsScriptClass() = default;

	public:
		void* get_native() const { return mp_class; }

		Bool has_method(String const& name, Int const parameterCount = 0) const override;

		Bool is_derived_from(Ref<ScriptClass> const klass) const override;

	protected:
		Owner<ScriptMethod> create_method(String const& name, Int const parameterCount) override;

	public:
		MonoClass* get_class() const { return mp_class; }
	};
}