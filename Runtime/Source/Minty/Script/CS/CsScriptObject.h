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
		void initialize_fields(std::unordered_map<String, Owner<ScriptField>>& fields) override;
		void initialize_properties(std::unordered_map<String, Owner<ScriptProperty>>& properties) override;
		void initialize_methods(std::unordered_map<String, Owner<ScriptMethod>>& methods) override;

	public:
		void* get_native() const override { return mp_object; }

		MonoObject* get_object() const { return mp_object; }
	};
}