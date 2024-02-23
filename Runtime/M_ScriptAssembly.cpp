#include "pch.h"
#include "M_ScriptAssembly.h"

#include "M_EntityRegistry.h"
#include "M_ScriptEngine.h"
#include "M_Mono.h"
#include "M_Console.h"
#include "M_File.h"

using namespace minty;

minty::ScriptAssembly::ScriptAssembly(Path const& path, ScriptEngine& engine, bool const referenceOnly)
	: _path(path)
	, _engine(&engine)
	, _assembly()
	, _image()
	, _classes()
{
	MINTY_ASSERT_FORMAT(std::filesystem::exists(_path), "Assembly not found at path \"{}\".", _path.string());

	Console::log(std::format("Loading Assembly at path: \"{}\"", _path.string()));
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
		Console::info(std::format("Found {}::{}.", name, fullName));
		_classes.emplace(fullName, ScriptClass(namespaceName, className, *this)).first->second;
	}
}

minty::ScriptAssembly::~ScriptAssembly()
{
	//mono_assembly_close(_assembly);

	_path = Text::EMPTY;
	_engine = nullptr;
	_assembly = nullptr;
	_classes.clear();
}

String minty::ScriptAssembly::get_name() const
{
	return _path.stem().string();
}

ScriptEngine& minty::ScriptAssembly::get_engine() const
{
	return *_engine;
}

ScriptClass const* minty::ScriptAssembly::get_class(String const& namespaceName, String const& className) const
{
	return get_class(ScriptEngine::get_full_name(namespaceName, className));
}

ScriptClass const* minty::ScriptAssembly::get_class(String const& fullName) const
{
	auto found = _classes.find(fullName);

	if (found != _classes.end())
	{
		return &found->second;
	}

	return nullptr;
}

std::vector<ScriptClass const*> minty::ScriptAssembly::get_classes(ScriptClass const* baseClass) const
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

ScriptClass const* minty::ScriptAssembly::search_for_class(String const& name) const
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
