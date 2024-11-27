#include "pch.h"
#include "CsScriptEngine.h"

#include "Minty/Core/Constants.h"
#include "Minty/Core/Operation.h"

using namespace Minty;

MonoDomain* CsScriptEngine::sp_rootDomain = nullptr;
MonoDomain* CsScriptEngine::sp_appDomain = nullptr;

void Minty::CsScriptEngine::initialize(ScriptEngineBuilder const& builder)
{
	Path currentPath = std::filesystem::current_path();
	String currentPathString = currentPath.generic_string();

	Path assemblyDir = currentPath / "mono" / "lib";
	Path configDir = currentPath / "mono" / "etc";
	mono_set_dirs(assemblyDir.string().c_str(), configDir.string().c_str());

	// change MONO_PATH from system default to our custom mono files
	Operation::set_environment_variable("MONO_PATH", assemblyDir.generic_string());

#ifdef MINTY_DEBUG
	// enable debugging properties
	//Operation::set_environment_variable("MONO_LOG_LEVEL", "debug");
	//Operation::set_environment_variable("MONO_LOG_MASK", "all");
	//mono_jit_set_trace_options("-all");
#endif // MINTY_DEBUG

	sp_rootDomain = mono_jit_init(MINTY_NAME_DOMAIN_ROOT);
	MINTY_ASSERT(sp_rootDomain != nullptr);

	mono_set_assemblies_path(assemblyDir.generic_string().c_str());

	String appDomainName = MINTY_NAME_DOMAIN_APP;
	sp_appDomain = mono_domain_create_appdomain(appDomainName.data(), nullptr);
	MINTY_ASSERT(sp_appDomain != nullptr);
	mono_domain_set(sp_appDomain, false);
}

void Minty::CsScriptEngine::shutdown()
{
	mono_domain_set(sp_rootDomain, false);

	if (sp_appDomain)
	{
		mono_domain_unload(sp_appDomain);
		sp_appDomain = nullptr;
	}

	if (sp_rootDomain)
	{
		mono_jit_cleanup(sp_rootDomain);
		sp_rootDomain = nullptr;
	}
}

String Minty::CsScriptEngine::get_exception_message(MonoObject* const exception)
{
	if (!exception) {
		return "";
	}

	MonoMethod* get_message_method = mono_class_get_method_from_name(mono_get_exception_class(), "get_Message", 0);
	if (!get_message_method) {
		//std::cerr << "Could not find the get_Message method." << std::endl;
		return "";
	}

	MonoObject* exc = nullptr;
	MonoObject* result = mono_runtime_invoke(get_message_method, exception, nullptr, &exc);
	if (exc) {
		// Handle any exception that might occur during the get_Message invocation
		//std::cerr << "Exception occurred while getting message." << std::endl;
		return "";
	}

	String out = "";

	if (result) {
		char* str = mono_string_to_utf8((MonoString*)result);
		//std::cout << "Exception message: " << str << std::endl;
		out = str;
		mono_free(str);
	}
	else {
		//std::cout << "No exception message." << std::endl;
	}

	return out;
}

Type Minty::CsScriptEngine::to_type(MonoType* const type)
{
	if (!type)
	{
		return Type::Undefined;
	}

	MonoTypeEnum typeEnum = static_cast<MonoTypeEnum>(mono_type_get_type(type));

	switch (typeEnum)
	{
	case MONO_TYPE_BOOLEAN:
		return Type::Bool;
	case MONO_TYPE_CHAR:
		return Type::Char;
	case MONO_TYPE_I1:
		return Type::Byte;
	case MONO_TYPE_U1:
		return Type::Byte;
	case MONO_TYPE_I2:
		return Type::Short;
	case MONO_TYPE_U2:
		return Type::UShort;
	case MONO_TYPE_I4:
		return Type::Int;
	case MONO_TYPE_U4:
		return Type::UInt;
	case MONO_TYPE_I8:
		return Type::Long;
	case MONO_TYPE_U8:
		return Type::ULong;
	case MONO_TYPE_R4:
		return Type::Float;
	case MONO_TYPE_R8:
		return Type::Double;
	case MONO_TYPE_STRING:
		return Type::String;
	default:
		return Type::Undefined;
	}
}

String Minty::CsScriptEngine::from_mono_string(MonoString* string)
{
	char* str = mono_string_to_utf8(string);
	String result(str);
	mono_free(str);
	return result;
}

MonoString* Minty::CsScriptEngine::to_mono_string(String const& string)
{
	return mono_string_new(sp_appDomain, string.c_str());
}
