#pragma once
#include "CS.h"

#include "Minty/Script/Accessibility.h"
#include "Minty/Script/ScriptObject.h"

namespace Minty
{
	class CsScriptObject
		: public ScriptObject
	{
	private:
		MonoObject* mp_object;
		uint32_t m_gcHandle;

	public:
		CsScriptObject(ScriptObjectBuilder const& builder);

		CsScriptObject(ScriptObjectBuilder const& builder, ScriptArguments& args);

		~CsScriptObject();

	protected:
		Owner<ScriptField> create_field(String const& name) override;

		Owner<ScriptProperty> create_property(String const& name) override;

		Owner<ScriptMethod> create_method(String const& name, Int const parameterCount) override;

	public:
		void* get_native() const override { return mp_object; }

		MonoObject* get_object() const { return mp_object; }
	};
}