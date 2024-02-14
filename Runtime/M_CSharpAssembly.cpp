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

void minty::CSharpAssembly::load(Path const& path)
{
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

	// TODO: remove test:
	MonoImage* assemblyImage = mono_assembly_get_image(_assembly);
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
