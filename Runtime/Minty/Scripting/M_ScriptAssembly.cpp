#include "pch.h"
#include "Minty/Scripting/M_ScriptAssembly.h"

#include "Minty/Entities/M_EntityRegistry.h"
#include "Minty/Scripting/M_ScriptEngine.h"
#include "Minty/Libraries/M_Mono.h"
#include "Minty/Tools/M_Console.h"
#include "Minty/Files/M_File.h"

using namespace Minty;

Minty::ScriptAssembly::ScriptAssembly(Path const& path, bool const referenceOnly)
	: _path(path)
	, _refOnly(referenceOnly)
	, _assembly()
	, _image()
	, _classes()
{
	MINTY_ASSERT_FORMAT(std::filesystem::exists(_path), "Assembly not found at path \"{}\".", _path.string());

	MINTY_LOG_FORMAT("Loading Assembly at path: \"{}\"", _path.string());
	std::vector<char> fileData = File::read_all_chars(_path);

	// load the assembly image
	MonoImageOpenStatus status;
	MonoImage* loadImage = mono_image_open_from_data_full(fileData.data(), static_cast<uint32_t>(fileData.size()), 1, &status, referenceOnly);

	// check for error
	if (status != MONO_IMAGE_OK)
	{
		// get the error message
		char const* errorMessage = mono_image_strerror(status);
		MINTY_ABORT(std::format("ScriptEngine::load_mono_assembly(): Mono error: \"{}\"", errorMessage));

		return;
	}

	// load the assembly itself
	_assembly = mono_assembly_load_from_full(loadImage, _path.string().c_str(), &status, referenceOnly);
	mono_image_close(loadImage);

	// load all of the classes from the assembly
	_image = mono_assembly_get_image(_assembly);
	const MonoTableInfo* tableInfo = mono_image_get_table_info(_image, MONO_TABLE_TYPEDEF);
	int rows = mono_table_info_get_rows(tableInfo);

	String name = get_name();

	for (int i = 1; i < rows; i++)
	{
		// get the mono class
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(tableInfo, i, cols, MONO_TYPEDEF_SIZE);
		char const* className = mono_metadata_string_heap(_image, cols[MONO_TYPEDEF_NAME]);
		char const* namespaceName = mono_metadata_string_heap(_image, cols[MONO_TYPEDEF_NAMESPACE]);
		String fullName = ScriptEngine::get_full_name(namespaceName, className);
		MonoClass* klass = mono_class_from_name(_image, namespaceName, className);

		// create script class and add to list
		MINTY_INFO_FORMAT("Found {}::{}.", name, fullName);
		_classes.emplace(fullName, ScriptClass(namespaceName, className, *this)).first->second;
	}

	// print dependencies for now
	MINTY_INFO("Dependencies:");
	for (String const& name : get_dependencies())
	{
		MINTY_INFO(name);
	}
}

Minty::ScriptAssembly::~ScriptAssembly()
{
	//mono_assembly_close(_assembly);

	_path = "";
	_assembly = nullptr;
	_classes.clear();
}

ScriptClass const* Minty::ScriptAssembly::get_class(String const& namespaceName, String const& className) const
{
	return get_class(ScriptEngine::get_full_name(namespaceName, className));
}

ScriptClass const* Minty::ScriptAssembly::get_class(String const& fullName) const
{
	auto found = _classes.find(fullName);

	if (found != _classes.end())
	{
		return &found->second;
	}

	return nullptr;
}

std::vector<ScriptClass const*> Minty::ScriptAssembly::get_classes(ScriptClass const* baseClass) const
{
	std::vector<ScriptClass const*> classes;

	for (auto const& pair : _classes)
	{
		if (!baseClass || pair.second.is_derived_from(*baseClass))
		{
			classes.push_back(&pair.second);
		}
	}

	return classes;
}

ScriptClass const* Minty::ScriptAssembly::search_for_class(String const& name) const
{
	for (auto const& pair : _classes)
	{
		if (pair.second.get_name() == name)
		{
			return &pair.second;
		}
	}

	return nullptr;
}

std::unordered_set<String> Minty::ScriptAssembly::get_dependencies() const
{
	std::unordered_set<String> results;

	// always include System
	results.emplace("System");

	int count = mono_image_get_table_rows(_image, MONO_TABLE_ASSEMBLYREF);

	for (int i = 0; i < count; i++)
	{
		uint32_t cols[MONO_ASSEMBLYREF_SIZE];
		mono_metadata_decode_row(mono_image_get_table_info(_image, MONO_TABLE_ASSEMBLYREF), i, cols, MONO_ASSEMBLYREF_SIZE);

		const char* name = mono_metadata_string_heap(_image, cols[MONO_ASSEMBLYREF_NAME]);
		results.emplace(name);
	}

	return results;
}
