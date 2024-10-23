#include "pch.h"
#include "CsScriptAssembly.h"

#include "Minty/Asset/AssetManager.h"
#include "Minty/Core/Linker.h"
#include "Minty/File/File.h"
#include "Minty/Script/ScriptEngine.h"
#include "Minty/Script/CS/CsScriptClass.h"

using namespace Minty;

Minty::CsScriptAssembly::CsScriptAssembly(ScriptAssemblyBuilder const& builder)
	: ScriptAssembly(builder)
	, mp_assembly(nullptr)
{
	String name = builder.path.stem().generic_string();
	MINTY_INFO_FORMAT("Loading {}.", name);

	// read meta, get name <-> ID pairs
	std::unordered_map<String, UUID> nameToId;
	{
		Path metaPath = Asset::get_meta_path(builder.path);

		Container* container;
		Reader* reader;
		AssetManager::open_reader(metaPath, container, reader);

		nameToId.reserve(reader->size());
		String name;
		UUID id;
		for (Size i = 0; i < reader->size(); i++)
		{
			// name is the class name, value is the id
			reader->read_name(i, name);
			reader->read(i, id);
			nameToId.emplace(name, id);
		}

		AssetManager::close_reader(container, reader);
	}
	
	// read file contents
	std::vector<Char> fileData = AssetManager::read_file_chars(builder.path);

	// load assembly image
	MonoImageOpenStatus status;
	MonoImage* loadImage = mono_image_open_from_data_full(fileData.data(), static_cast<uint32_t>(fileData.size()), 1, &status, builder.referenceOnly);

	// check for error
	if (status != MONO_IMAGE_OK)
	{
		// get error message
		char const* errorMessage = mono_image_strerror(status);
		MINTY_ABORT(std::format("ScriptEngine::load_mono_assembly(): Mono error: \"{}\"", errorMessage));

		return;
	}

	// load the assembly
	mp_assembly = mono_assembly_load_from_full(loadImage, builder.path.string().c_str(), &status, builder.referenceOnly);
	mono_image_close(loadImage);

	// load all of the classes from the assembly
	MonoImage* image = mono_assembly_get_image(mp_assembly);
	const MonoTableInfo* tableInfo = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
	int rows = mono_table_info_get_rows(tableInfo);

	String className;
	String namespaceName;
	String fullName;
	UUID id;
	for (int i = 1; i < rows; i++)
	{
		// get the mono class
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(tableInfo, i, cols, MONO_TYPEDEF_SIZE);
		className = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
		namespaceName = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
		fullName = ScriptEngine::get_full_name(namespaceName, className);

		auto found = nameToId.find(fullName);

		if (found == nameToId.end())
		{
			id = UUID::create();

			MINTY_WARN_FORMAT("No ID found for {}::{}. Setting to new ID: {}.", name, fullName, to_string(id));
		}
		else
		{
			id = found->second;

			MINTY_INFO_FORMAT("Found {}::{} ({}).", name, fullName, to_string(id));
		}

		// create script class and add to list
		ScriptClassBuilder scriptClassBuilder{};
		scriptClassBuilder.assembly = create_ref();
		scriptClassBuilder.namespaceName = namespaceName;
		scriptClassBuilder.className = className;
		scriptClassBuilder.id = id;
		Owner<CsScriptClass> scriptClass = Owner<CsScriptClass>(scriptClassBuilder);
		Ref<ScriptClass> scriptClassRef = scriptClass.create_ref();
		emplace_class(scriptClass);

		// register script for usage
		Linker::register_script(scriptClassRef);
	}

	//// print dependencies for now
	//MINTY_INFO("Dependencies:");
	//for (String const& name : get_dependencies())
	//{
	//	MINTY_INFO(name);
	//}
}

Minty::CsScriptAssembly::~CsScriptAssembly()
{
}

MonoImage* Minty::CsScriptAssembly::get_image() const
{
	return mono_assembly_get_image(mp_assembly);
}
