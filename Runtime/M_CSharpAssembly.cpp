#include "pch.h"
#include "M_CSharpAssembly.h"

#include "M_Console.h"
#include "M_File.h"

using namespace minty;

minty::CSharpAssembly::CSharpAssembly()
	: _rootDomain()
	, _appDomain()
	, _assembly()
{

	mono_set_assemblies_path("C:/Libraries/Mono/lib");

	_rootDomain = mono_jit_init("MyScriptRuntime");
	if (!_rootDomain)
	{
		Console::error("Failed to init Mono root domain.");
		return;
	}

	char appDomainName[] = "MyAppDomain";
	_appDomain = mono_domain_create_appdomain(appDomainName, nullptr);
	mono_domain_set(_appDomain, true);
}

minty::CSharpAssembly::~CSharpAssembly()
{}

void print_assembly(MonoAssembly* const assembly)
{
	MonoImage* assemblyImage = mono_assembly_get_image(assembly);
	MonoTableInfo const* typeDefinitionsTable = mono_image_get_table_info(assemblyImage, MONO_TABLE_TYPEDEF);
	int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

	Console::log("Found CSharp Assembly With:");
	for (int32_t i = 0; i < numTypes; i++)
	{
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

		char const* nameSpace = mono_metadata_string_heap(assemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
		char const* name = mono_metadata_string_heap(assemblyImage, cols[MONO_TYPEDEF_NAME]);

		Console::log(std::format("{}.{}", nameSpace, name));
	}
	Console::log("End CSharp Assembly.");
}

void minty::CSharpAssembly::load(Path const& path)
{
	Console::log(std::format("Loading CSharpAssembly at path: {}", path.string()));
	std::vector<char> fileData = File::read_all_chars(path);

	bool const refOnly = false; // set refonly to true to do reflection stuffs

	// load the assembly image
	MonoImageOpenStatus status;
	MonoImage* image = mono_image_open_from_data_full(fileData.data(), static_cast<uint32_t>(fileData.size()), 1, &status, refOnly);

	// check for error
	if (status != MONO_IMAGE_OK)
	{
		// get the error message
		char const* errorMessage = mono_image_strerror(status);

		Console::error(std::format("Runtime::load_csharp_assembly(): Mono error: \"{}\"", errorMessage));

		_assembly = nullptr;
		return;
	}

	// load the assembly itself
	_assembly = mono_assembly_load_from_full(image, path.string().c_str(), &status, refOnly);
	mono_image_close(image);

	print_assembly(_assembly);

	MonoClass* testingClass = get_class("", "CSharpTesting");

	MonoObject* testingObject = create_instance(testingClass);

	MonoMethod* printFloatVarMethod = get_method(testingClass, "PrintFloatVar");

	invoke_method(testingObject, printFloatVarMethod);

	float value = 5.0f;
	std::vector<void*> args = { &value };
	invoke_method(testingObject, get_method(testingClass, "IncrementFloatVar", 1), args);

	invoke_method(testingObject, printFloatVarMethod);

	MonoClassField* myPublicFloatVarField = get_field(testingClass, "MyPublicFloatVar");

	value = 0.0f;
	get_field_value(testingObject, myPublicFloatVarField, &value);

	value += 10;
	set_field_value(testingObject, myPublicFloatVarField, &value);

	invoke_method(testingObject, printFloatVarMethod);

	/*
	MonoProperty* nameProperty = mono_class_get_property_from_name(testingClass, "Name");

	// Get the value of Name by invoking the getter method
	MonoString* nameValue = (MonoString*)mono_property_get_value(nameProperty, testingInstance, nullptr, nullptr);
	std::string nameStr = MonoStringToUTF8(nameValue);

	// Modify and assign the value back to the property by invoking the setter method
	nameStr += ", World!";
	nameValue = mono_string_new(s_AppDomain, nameStr.c_str());
	mono_property_set_value(nameProperty, testingInstance, (void**)&nameValue, nullptr);
	*/
}

MonoClass* minty::CSharpAssembly::get_class(String const& namespaceName, String const& className) const
{
	MonoImage* image = mono_assembly_get_image(_assembly);
	MonoClass* klass = mono_class_from_name(image, namespaceName.c_str(), className.c_str());

	if (!klass)
	{
		Console::error(std::format("CSharpAssembly::get_class(): Failed to get class \"{}.{}\".", namespaceName, className));
		return nullptr;
	}

	return klass;
}

MonoMethod* minty::CSharpAssembly::get_method(MonoClass* const klass, String const& methodName, int const paramCount) const
{
	MonoMethod* method = mono_class_get_method_from_name(klass, methodName.c_str(), paramCount);

	if (!method)
	{
		Console::error(std::format("CSharpAssembly::get_method(): Method \"{}\" with {} parameters does not exist.", methodName, paramCount));
		return nullptr;
	}
	
	return method;
}

MonoClassField* minty::CSharpAssembly::get_field(MonoClass* const klass, String const& fieldName) const
{
	MonoClassField* field = mono_class_get_field_from_name(klass, fieldName.c_str());

	if (!field)
	{
		Console::error(std::format("CSharpAssembly::get_field(): Field \"{}\" does not exist.", fieldName));
		return nullptr;
	}

	return field;
}

void minty::CSharpAssembly::get_field_value(MonoObject* const object, MonoClassField* const field, void* out) const
{
	mono_field_get_value(object, field, out);
}

void minty::CSharpAssembly::set_field_value(MonoObject* const object, MonoClassField* const field, void* in) const
{
	mono_field_set_value(object, field, in);
}

MonoProperty* minty::CSharpAssembly::get_property(MonoClass* const klass, String const& propertyName) const
{
	MonoProperty* prop = mono_class_get_property_from_name(klass, propertyName.c_str());

	if (!prop)
	{
		Console::error(std::format("CSharpAssembly::get_field(): Field \"{}\" does not exist.", propertyName));
		return nullptr;
	}

	return prop;
}

void minty::CSharpAssembly::invoke_method(MonoObject* const object, MonoMethod* const method) const
{
	std::vector<void*> empty;
	invoke_method(object, method, empty);
}

void minty::CSharpAssembly::invoke_method(MonoObject* const object, MonoMethod* const method, std::vector<void*>& arguments) const
{
	MonoObject* exception = nullptr;
	mono_runtime_invoke(method, object, arguments.data(), &exception);

	if (exception)
	{
		Console::error("CSharpAssembly::invoke_method(): An exception was thrown.");
		return;
	}
}

MonoObject* minty::CSharpAssembly::create_instance(MonoClass* const klass) const
{
	// create the instance
	MonoObject* instance = mono_object_new(_appDomain, klass);

	if (!instance)
	{
		Console::error("CSharpAssembly::create_instance(): Failed.");
		return nullptr;
	}

	// call the constructor
	mono_runtime_object_init(instance);

	return instance;
}
