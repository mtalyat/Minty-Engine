#include "pch.h"
#include "AssetManager.h"

#include "Minty/Core/Application.h"
#include "Minty/Core/Console.h"
#include "Minty/Core/ToString.h"
#include "Minty/Entity/EntityRegistry.h"
#include "Minty/File/File.h"
#include "Minty/File/PhysicalFile.h"
#include "Minty/File/VirtualFile.h"
// this define has to be here, above Stb.h, in a cpp source file
#define STB_IMAGE_IMPLEMENTATION
#include "Minty/Library/Stb.h"
#include "Minty/Render/Renderer.h"
#include "Minty/Render/ShaderModule.h"
#include "Minty/Script/ScriptEngine.h"
#include "Minty/System/SystemRegistry.h"

using namespace Minty;

#define MINTY_ASSERT_ASSET_PATH(path) MINTY_ASSERT_MESSAGE(!(path).empty(), "Asset path is empty."); \
MINTY_ASSERT_FORMAT(exists(path), "Asset path does not exist: \"{}\"", (path).generic_string()); \
MINTY_ASSERT_FORMAT(exists(Asset::get_meta_path(path)), "Asset meta path does not exist: \"{}\"", Asset::get_meta_path(path).generic_string());

AssetMode Minty::AssetManager::s_mode = {};
Bool Minty::AssetManager::s_savePaths = false;
std::unordered_map<UUID, AssetManager::AssetData> Minty::AssetManager::s_assets = {};
std::unordered_map<AssetType, std::unordered_set<Ref<Asset>>> Minty::AssetManager::s_assetsByType = {};
Wrapper Minty::AssetManager::s_wrapper = {};

void Minty::AssetManager::initialize(const AssetManagerBuilder& builder)
{
	// set values
	s_mode = builder.mode;
	s_assets = {};
	s_assetsByType = {};
	s_wrapper = {};

	// load all of the wrap files given in the builder
	for (const Path& path : builder.wraps)
	{
		s_wrapper.emplace(path);
	}
}

void Minty::AssetManager::shutdown()
{
	unload_all();
}

File* Minty::AssetManager::open(const Path& path)
{
	File* file = nullptr;

	if (s_mode == AssetMode::ReadFiles || (s_mode == AssetMode::ReadAll && std::filesystem::exists(path)))
	{
		file = new PhysicalFile();
		file->open(path, File::Flags::Read);
	}

	if (s_mode == AssetMode::ReadWraps || (s_mode == AssetMode::ReadAll && s_wrapper.contains(path)))
	{
		file = new VirtualFile();
		s_wrapper.open(path, *static_cast<VirtualFile*>(file));
	}

	return file;
}

Bool Minty::AssetManager::open_reader(const Path& path, Container*& container, Reader*& reader)
{
	std::vector<Char> data = read_file_chars(path);

	if (data.size() == 0) return false;

	container = new ConstantContainer(data.size());
	container->set(data.data(), data.size());

	reader = new TextMemoryReader(container);

	return true;
}

void Minty::AssetManager::close_reader(Container*& container, Reader*& reader)
{
	delete reader;
	reader = nullptr;
	delete container;
	container = nullptr;
}

UUID Minty::AssetManager::read_id(const Path& path)
{
	Path metaPath = Asset::get_meta_path(path);

	Container* container = nullptr;
	Reader* reader = nullptr;
	if (open_reader(metaPath, container, reader))
	{
		Node const& root = reader->get_current_node();
		String rootDataString = root.get_data_as_string();
		UUID id = Parse::to_uuid(rootDataString);
		close_reader(container, reader);

		// able to read
		return id;
	}

	// unable to read
	return UUID();
}

Bool Minty::AssetManager::exists(const Path& path)
{
	switch (s_mode)
	{
	case AssetMode::ReadFiles:
		return std::filesystem::exists(path);
	case AssetMode::ReadWraps:
		return s_wrapper.contains(path);
	case AssetMode::ReadAll:
		return s_wrapper.contains(path) || std::filesystem::exists(path);
	default:
		MINTY_ABORT("Unrecognized AssetMode.");
	}
}

Ref<Asset> Minty::AssetManager::load_asset(const Path& path)
{
	// get the asset type
	AssetType type = Asset::get_type_from_path(path);

	if (type == AssetType::None) return nullptr;

	// load based on the type
	switch (type)
	{
	case AssetType::Animation:
		return load_animation(path);
	case AssetType::Animator:
		return load_animator(path);
	case AssetType::Material:
		return load_material(path);
	case AssetType::MaterialTemplate:
		return load_material_template(path);
	case AssetType::Scene:
		return load_scene(path);
	case AssetType::Shader:
		return load_shader(path);
	case AssetType::ShaderModule:
		return load_shader_module(path);
	case AssetType::Sprite:
		return load_sprite(path);
	case AssetType::Texture:
		return load_texture(path);
	default:
		MINTY_ERROR_FORMAT("Asset loading not implemented for type: {}", to_string(type));
		return nullptr;
	}
}

void Minty::AssetManager::emplace_by_type(Ref<Asset> const asset)
{
	AssetType type = asset->get_type();

	auto found = s_assetsByType.find(type);

	if (found != s_assetsByType.end())
	{
		// add to existing set
		found->second.emplace(asset);
	}
	else
	{
		// create new set
		s_assetsByType.emplace(type, std::unordered_set<Ref<Asset>>());
		s_assetsByType.at(type).emplace(asset);
	}
}

void Minty::AssetManager::erase_by_type(Ref<Asset> const asset)
{
	AssetType type = asset->get_type();

	auto found = s_assetsByType.find(type);

	if (found != s_assetsByType.end())
	{
		// erase from type set
		found->second.erase(asset);

		// remove type set if empty
		if (found->second.empty())
		{
			s_assetsByType.erase(type);
		}
	}
}

void Minty::AssetManager::unload(const UUID id)
{
	auto found = s_assets.find(id);

	if (found == s_assets.end())
	{
		return;
	}

	// remove the reference
	erase(id);
}

void Minty::AssetManager::unload_all()
{
	// get all IDs into a vector that can be iterated over
	std::vector<UUID> ids;
	ids.reserve(s_assets.size());
	for (auto const& [uuid, asset] : s_assets)
	{
		ids.push_back(uuid);
	}

	// iterate each id: erase and delete each asset
	Ref<Asset> asset;
	for (auto const id : ids)
	{
		if (asset == get_asset(id))
		{
			erase(id);
		}
	}

	s_assets.clear();
	s_assetsByType.clear();
}

void Minty::AssetManager::emplace(const Path& path, const Owner<Asset> asset)
{
	MINTY_ASSERT_MESSAGE(asset.get() != nullptr, "Cannot emplace a null asset into the AssetManager.");
	MINTY_ASSERT_FORMAT(!s_assets.contains(asset->id()), "An asset with the ID {} already exists and is loaded.", to_string(asset->id()));
	MINTY_ASSERT_MESSAGE(asset->id().valid(), "Found invalid asset ID when attempting to emplace Asset into the AssetManager.");

	AssetData assetData{};
	assetData.asset = asset;

	if (s_savePaths)
	{
		assetData.path = path;
	}

	s_assets.emplace(asset->id(), assetData);
	emplace_by_type(asset);

	asset->initialize();
}

Bool Minty::AssetManager::erase(const UUID id)
{
	MINTY_ASSERT(s_assets.contains(id));
	Ref<Asset> asset = s_assets.at(id).asset.create_ref();
	asset->shutdown();
	erase_by_type(asset);
	return static_cast<Bool>(s_assets.erase(id));
}

Ref<Asset> Minty::AssetManager::get_asset(UUID const id)
{
	auto found = s_assets.find(id);

	if (found == s_assets.end())
	{
		return nullptr;
	}

	return found->second.asset.create_ref();
}

Ref<Asset> Minty::AssetManager::at_asset(UUID const id)
{
	return s_assets.at(id).asset.create_ref();
}

Path Minty::AssetManager::get_path(UUID const id)
{
	auto found = s_assets.find(id);

	if (found == s_assets.end())
	{
		return Path();
	}

	return found->second.path;
}

Bool Minty::AssetManager::contains(const UUID id)
{
	return s_assets.contains(id);
}

String Minty::AssetManager::read_file(Path const& path)
{
	// read file as chars
	std::vector<Char> chars = read_file_chars(path);

	// add null char for good measure
	chars.push_back('\0');

	// turn into string
	return String(chars.data());
}

std::vector<Char> Minty::AssetManager::read_file_chars(Path const& path)
{
	switch (s_mode)
	{
	case AssetMode::ReadFiles:
		return File::read_all_chars(path);
	case AssetMode::ReadWraps:
		return s_wrapper.read(path);
	case AssetMode::ReadAll:
		if (std::filesystem::exists(path))
		{
			return File::read_all_chars(path);
		}
		else
		{
			return s_wrapper.read(path);
		}
	default:
		MINTY_ABORT("Unrecognized AssetMode.");
	}
}

std::vector<Byte> Minty::AssetManager::read_file_bytes(Path const& path)
{
	std::vector<Char> data = read_file_chars(path);

	std::vector<Byte> result(data.size());

	memcpy(result.data(), data.data(), sizeof(Char) * data.size());

	return result;
}

std::vector<String> Minty::AssetManager::read_file_lines(Path const& path)
{
	std::vector<Char> data = read_file_chars(path);

	std::vector<String> result;

	std::istringstream ss(String(data.data(), sizeof(Char) * data.size()));

	String line;
	while (std::getline(ss, line))
	{
		// remove the \r
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

		result.push_back(line);
	}

	return result;
}

Bool Minty::AssetManager::check_dependency(Path const& path, String const& name, const UUID id)
{
	// missing dependency IF one of these:
	// - does not contain dependency: ID DNE
	// - does not contain dependency: no asset loaded with id
	if (!id.valid() || !contains(id))
	{
		missing_dependency(path, name, id);
		return false;
	}

	// dependency is there
	return true;
}

void Minty::AssetManager::missing_dependency(Path const& path, String const& name, UUID const id)
{
	Minty::Debug::log_error(std::format("Cannot load \"{}\". Missing \"{}\" dependency with ID {}.", path.generic_string(), name, to_string(id)));
}

Ref<Animation> Minty::AssetManager::load_animation(const Path& path)
{
	MINTY_ASSERT_ASSET_PATH(path);

	AnimationBuilder builder{};
	builder.id = read_id(path);

	Container* container;
	Reader* reader;
	open_reader(path, container, reader);

	reader->read("length", builder.length);
	reader->read("loops", builder.loops);
	reader->read("entities", builder.entities);
	reader->read("components", builder.components);
	reader->read("variables", builder.variables);
	reader->read("values", builder.values);

	// custom logic for steps
	if (reader->indent("steps"))
	{
		// read each step
		String timeString;
		Float time;
		String stepString;
		for (Size i = 0; i < reader->size(); i++)
		{
			// read time
			reader->read_name(i, timeString);
			time = Parse::to_float(timeString);

			// read steps
			reader->indent(i);
			std::vector<AnimationStep> steps(reader->size());
			for (Size j = 0; j < reader->size(); j++)
			{
				reader->read_name(j, stepString);
				AnimationStep& step = steps.at(j);
				step = Animation::parse_step(stepString);
			}
			reader->outdent();

			// add to steps
			builder.steps.emplace(time, std::move(steps));
		}

		reader->outdent();
	}

	// custom logic for reset steps
	if (reader->indent("reset"))
	{
		String stepString;
		builder.resetSteps.resize(reader->size());
		for (Size i = 0; i < reader->size(); i++)
		{
			reader->read_name(i, stepString);
			AnimationStep& step = builder.resetSteps.at(i);
			step = Animation::parse_step(stepString);
		}

		reader->outdent();
	}

	close_reader(container, reader);

	return create_existing<Animation>(path, builder);
}

Ref<Animator> Minty::AssetManager::load_animator(const Path& path)
{
	MINTY_ASSERT_ASSET_PATH(path);

	AnimatorBuilder builder{};
	builder.id = read_id(path);

	Container* container;
	Reader* reader;

	open_reader(path, container, reader);

	// basically just read the FSM
	reader->read("fsm", builder.fsm);

	close_reader(container, reader);

	return create_existing<Animator>(path, builder);
}

Owner<Image> Minty::AssetManager::load_image(const Path& path)
{
	MINTY_ASSERT_ASSET_PATH(path);

	// get file data
	std::vector<Byte> fileData = read_file_bytes(path);

	// get pixels from file data
	int width, height, channels;
	stbi_uc* rawPixels = stbi_load_from_memory(fileData.data(), static_cast<int>(fileData.size()), &width, &height, &channels, static_cast<int>(ImagePixelFormat::RedGreenBlueAlpha));

	MINTY_ASSERT_MESSAGE(rawPixels != nullptr, "Failed to load image pixel data.");

	// create the image
	ImageBuilder builder{};
	builder.format = Format::R8G8B8A8_SRGB;
	builder.type = ImageType::D2;
	builder.tiling = ImageTiling::Optimal;

	// copy pixels to builder
	builder.width = static_cast<UInt>(width);
	builder.height = static_cast<UInt>(height);
	builder.pixels.resize(static_cast<Size>(width) * static_cast<Size>(height) * 4);
	memcpy(builder.pixels.data(), rawPixels, builder.pixels.size());

	// free stb pixels
	stbi_image_free(rawPixels);

	return Image::create(builder);
}

Bool Minty::AssetManager::load_values(Reader& reader, std::unordered_map<String, Cargo>& values, Ref<Shader> const shader, Path const& path)
{
	String inputName;
	String valueName;

	for (Size i = 0; i < reader.size(); i++)
	{
		reader.read_name(i, inputName);

		MINTY_ASSERT_FORMAT(shader->has_input(inputName), "Shader does not have an input with the name \"{}\".", inputName);

		// reference input from shader settings, to ensure this matches shader
		ShaderInput const& input = shader->get_input(inputName);

		// get own copy of cargo for this input
		values.emplace(inputName, input.cargo);
		Cargo& cargo = values.at(inputName);

		reader.indent(i);

		for (Size j = 0; j < reader.size(); j++)
		{
			reader.read_name(j, valueName);

			MINTY_ASSERT_FORMAT(cargo.contains(valueName), "Shader does not have a value with the name \"{}\" for the input \"{}\".", valueName, inputName);

			Cargo::Slot& slot = cargo.at(valueName);

			// if pointer: get asset
			// if not: read directly
			if (slot.type == Type::Asset)
			{
				// read ID, then get asset ref, copy pointer to container
				Ref<Asset> asset{};
				if (!find_dependency(path, reader, valueName, asset))
				{
					return false;
				}
				Asset* assetPtr = asset.get();
				slot.container.set(&assetPtr, sizeof(assetPtr));
			}
			else
			{
				// read value directly into container
				reader.read_to_container(j, slot.container, slot.type);
			}
		}

		reader.outdent();
	}

	return true;
}

Ref<Material> Minty::AssetManager::load_material(Path const& path)
{
	MINTY_ASSERT_ASSET_PATH(path);

	MaterialBuilder builder{};
	builder.id = read_id(path);

	Container* container;
	Reader* reader;
	if (open_reader(path, container, reader))
	{
		if (!find_dependency<MaterialTemplate>(path, *reader, "Template", builder.materialTemplate))
		{
			close_reader(container, reader);
			return Ref<Material>();
		}

		if (reader->indent("Values"))
		{
			if (!load_values(*reader, builder.values, builder.materialTemplate->get_shader(), path))
			{
				close_reader(container, reader);
				return Ref<Material>();
			}

			reader->outdent();
		}

		close_reader(container, reader);
	}

	return create_existing<Material>(path, builder);
}

Ref<MaterialTemplate> Minty::AssetManager::load_material_template(Path const& path)
{
	MINTY_ASSERT_ASSET_PATH(path);

	MaterialTemplateBuilder builder{};
	builder.id = read_id(path);

	Container* container;
	Reader* reader;
	if (open_reader(path, container, reader))
	{
		if (!find_dependency<Shader>(path, *reader, "Shader", builder.shader))
		{
			close_reader(container, reader);
			return Ref<MaterialTemplate>();
		}

		if (reader->indent("Values"))
		{
			if (!load_values(*reader, builder.values, builder.shader, "MaterialTemplate"))
			{
				close_reader(container, reader);
				return Ref<MaterialTemplate>();
			}

			reader->outdent();
		}

		close_reader(container, reader);
	}

	return create_existing<MaterialTemplate>(path, builder);
}

Ref<Scene> Minty::AssetManager::load_scene(Path const& path)
{
	MINTY_ASSERT_ASSET_PATH(path);

	SceneBuilder builder{};
	builder.id = read_id(path);

	Container* container;
	Reader* reader;
	if (open_reader(path, container, reader))
	{
		if (!reader->read("Assets", builder.assets))
		{
			MINTY_ABORT("Scene missing \"Assets\".");
		}

		Ref<Scene> scene = create_existing<Scene>(path, builder);

		// set to working scene for easy access
		Application::instance().get_scene_manager().push_working_scene(scene);

		// systems
		if (reader->indent("Systems"))
		{
			SystemRegistry& registry = scene->get_system_registry();
			registry.deserialize(*reader);
			reader->outdent();
		}
		else
		{
			MINTY_ABORT("Scene missing \"Systems\".");
		}

		// entities
		if (reader->indent("Entities"))
		{
			EntityRegistry& registry = scene->get_entity_registry();
			registry.deserialize(*reader);
			reader->outdent();
		}
		else
		{
			MINTY_ABORT("Scene missing \"Entities\".");
		}

		close_reader(container, reader);

		// no longer working with the scene
		Application::instance().get_scene_manager().pop_working_scene();

		return scene;
	}

	// create Scene
	return create_existing<Scene>(path, builder);
}

Ref<Shader> Minty::AssetManager::load_shader(const Path& path)
{
	MINTY_ASSERT_ASSET_PATH(path);

	Container* container;
	Reader* reader;
	open_reader(path, container, reader);

	ShaderBuilder builder{};
	builder.id = read_id(path);

	// config
	reader->read("PrimitiveTopology", builder.primitiveTopology);
	reader->read("PolygonMode", builder.polygonMode);
	reader->read("CullMode", builder.cullMode);
	reader->read("FrontFace", builder.frontFace);
	reader->read("LineWidth", builder.lineWidth);

	// inputs (uniform, push, etc)
	if (reader->indent("Inputs"))
	{
		// for push constants only
		UInt offset = 0;

		builder.inputs.resize(reader->size());
		for (Size i = 0; i < reader->size(); i++)
		{
			ShaderInput& input = builder.inputs.at(i);

			// get name
			reader->read_name(i, input.name);

			// step in to read values
			reader->indent(i);

			// get basic data
			input.name = reader->name();
			reader->read("Binding", input.binding);
			reader->read("Stage", input.stage);
			reader->read("Type", input.type);
			reader->read("Count", input.count);
			reader->read("Frequent", input.frequent);

			// set offset if push const
			if (input.type == ShaderInputType::PushConstant)
			{
				input.offset = offset;
			}

			// get structure
			if (reader->indent("Structure"))
			{
				// each node follows this format:
				// name: Type
				String name = "";
				Type type = Type::Undefined;

				for (Size i = 0; i < reader->size(); i++)
				{
					// get name
					reader->read_name(i, name);

					// get type
					reader->read(i, type);
				}

				// emplace slot
				input.cargo.emplace_slot(name, type);

				// add to total size
				UInt typeSize = static_cast<UInt>(sizeof_type(type));
				input.size += typeSize;

				// step out of Structure
				reader->outdent();
			}

			// adjust offset if push const, so next push const is aligned
			if (input.type == ShaderInputType::PushConstant)
			{
				offset += input.size;
			}

			// step out of input
			reader->outdent();
		}

		// step out of Inputs
		reader->outdent();
	}

	// bindings
	if (reader->indent("Bindings"))
	{
		// allot space for each binding
		builder.vertexInput.bindings.resize(reader->size());

		for (Size i = 0; i < reader->size(); i++)
		{
			reader->indent(i);

			ShaderBinding& shaderBinding = builder.vertexInput.bindings.at(i);

			// allot space for each attribute
			shaderBinding.attributes.resize(reader->size());

			for (Size j = 0; j < reader->size(); j++)
			{
				ShaderAttribute& shaderAttribute = shaderBinding.attributes.at(j);

				// get attribute type
				reader->read(j, shaderAttribute.type);
			}

			reader->outdent();
		}

		reader->outdent();
	}

	// modules
	if (reader->indent("Stages"))
	{
		if (!find_dependency<ShaderModule>(path, *reader, "Vertex", builder.vertexShaderModule))
		{
			close_reader(container, reader);
			return Ref<Shader>();
		}
		if (!find_dependency<ShaderModule>(path, *reader, "Fragment", builder.fragmentShaderModule))
		{
			close_reader(container, reader);
			return Ref<Shader>();
		}

		reader->outdent();
	}

	// viewport and scissor
	if (!find_dependency<Viewport>(path, *reader, "Viewport", builder.viewport))
	{
		// default
		builder.viewport = Renderer::get_viewport();
	}
	if (!find_dependency<Scissor>(path, *reader, "Scissor", builder.scissor))
	{
		// default
		builder.scissor = Renderer::get_scissor();
	}

	close_reader(container, reader);

	return create_existing<Shader>(path, builder);
}

Ref<ShaderModule> Minty::AssetManager::load_shader_module(const Path& path)
{
	MINTY_ASSERT_ASSET_PATH(path);

	ShaderModuleBuilder builder{};
	builder.id = read_id(path);
	builder.path = path;

	return create_existing<ShaderModule>(path, builder);
}

Ref<Sprite> Minty::AssetManager::load_sprite(Path const& path)
{
	MINTY_ASSERT_ASSET_PATH(path);

	SpriteBuilder builder{};
	builder.id = read_id(path);

	Container* container;
	Reader* reader;
	open_reader(path, container, reader);

	if (!find_dependency(path, *reader, "texture", builder.texture))
	{
		// default
		builder.texture = get<Texture>(DEFAULT_TEXTURE);

		MINTY_ASSERT_MESSAGE(builder.texture != nullptr, "Default Texture not loaded.");
	}
	reader->read("offset", builder.offset);
	reader->read("size", builder.size);
	reader->read("pivot", builder.pivot);
	reader->read("ppu", builder.pixelsPerUnit);

	close_reader(container, reader);

	return create_existing<Sprite>(path, builder);
}

Ref<Texture> Minty::AssetManager::load_texture(const Path& path)
{
	MINTY_ASSERT_ASSET_PATH(path);

	TextureBuilder builder{};
	builder.id = read_id(path);
	builder.image = load_image(path);

	return create_existing<Texture>(path, builder);
}
