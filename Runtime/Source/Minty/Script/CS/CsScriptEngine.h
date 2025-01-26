#pragma once
#include "CS.h"

#include "Minty/Script/ScriptEngine.h"
#include <unordered_map>

namespace Minty
{
	class CsScriptEngine
	{
	private:
		static MonoDomain* sp_rootDomain;
		static MonoDomain* sp_appDomain;

	private:
		CsScriptEngine() = default;
		~CsScriptEngine() = default;

	public:
		static void initialize(ScriptEngineBuilder const& builder);

		static void shutdown();

	public:
		static MonoDomain* get_root_domain() { return sp_rootDomain; }

		static MonoDomain* get_app_domain() { return sp_appDomain; }

		static String get_exception_message(MonoObject* const exception);

		static Type to_type(MonoType* const type);

		static String from_mono_string(MonoString* string);

		static MonoString* to_mono_string(String const& string);

		static Bool check_equal(MonoClass* const left, MonoClass* const right);

		static UUID get_uuid(MonoObject* const object);

		static UUID get_entity_uuid(MonoObject* const object);

		static Accessibility get_field_accessibility(MonoClassField* const field);

		static Accessibility get_method_accessibility(MonoMethod* const method);
	};
}