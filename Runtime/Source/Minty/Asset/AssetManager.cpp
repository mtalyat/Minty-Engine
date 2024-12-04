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

void Minty::AssetManager::initialize(AssetManagerBuilder const& builder)
{
	// set values
	s_mode = builder.mode;
	s_assets = {};
	s_assetsByType = {};
	s_wrapper = {};
	s_savePaths = builder.savePaths;

	// load all of the wrap files given in the builder
	for (Path const& path : builder.wraps)
	{
		s_wrapper.emplace(path);
	}
}

void Minty::AssetManager::shutdown()
{
	unload_all();
}

File* Minty::AssetManager::open(Path const& path)
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

Bool Minty::AssetManager::open_reader(Path const& path, Container*& container, Reader*& reader)
{
	if (!exists(path))
	{
		// return false if it does not exist at all
		return false;
	}

	std::vector<Char> data = read_file_chars(path);

	// create empty, or populate if not empty
	if (data.empty())
	{
		container = new ConstantContainer();
	}
	else
	{
		container = new ConstantContainer(data.size());
		container->set(data.data(), data.size());
	}

	reader = new TextMemoryReader(container);

	return true;
}

void Minty::AssetManager::close_reader(Container*& container, Reader*& reader)
{
	MINTY_ASSERT_MESSAGE(container != nullptr, "Expecting a non-null Container in close_reader().");
	MINTY_ASSERT_MESSAGE(reader != nullptr, "Expecting a non-null Reader in close_reader().");

	delete reader;
	reader = nullptr;
	delete container;
	container = nullptr;
}

UUID Minty::AssetManager::read_id(Path const& path)
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

Bool Minty::AssetManager::exists(Path const& path)
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

Ref<Asset> Minty::AssetManager::load_asset(Path const& path)
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
	case AssetType::Font:
		return load_font(path);
	case AssetType::FontVariant:
		return load_font_variant(path);
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

void Minty::AssetManager::unload(UUID const id)
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

void Minty::AssetManager::emplace(Path const& path, Owner<Asset> const asset)
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

Bool Minty::AssetManager::erase(UUID const id)
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

Path Minty::AssetManager::get_path(UUID const id)
{
	if (!s_savePaths)
	{
		Debug::log_warning(std::format("AssetManager is not saving paths. ID: {}", to_string(id)));
		return Path();
	}

	auto found = s_assets.find(id);

	if (found == s_assets.end())
	{
		return Path();
	}

	return found->second.path;
}

String Minty::AssetManager::get_name(UUID const id)
{
	return get_path(id).stem().string();
}

Bool Minty::AssetManager::contains(UUID const id)
{
	return s_assets.contains(id);
}

std::vector<Ref<Asset>> Minty::AssetManager::get_dependents(Ref<Asset> const asset)
{
	std::vector<Ref<Asset>> result;

	// get type
	AssetType type = asset->get_type();

	// check based on the type
	// some types inherently have no dependents
	switch (type)
	{
	case AssetType::Shader:
		// MaterialTemplates use Shaders
		for (auto const materialTemplate : get_by_type<MaterialTemplate>())
		{
			if (materialTemplate->get_shader() == asset)
			{
				// the materialTemplate uses this shader
				result.push_back(materialTemplate);
			}
		}
		break;
	case AssetType::MaterialTemplate:
		// Materials use MaterialTemplates
		for (auto const material : get_by_type<Material>())
		{
			if (material->get_template() == asset)
			{
				result.push_back(material);
			}
		}
		break;
	case AssetType::Material:
		// TODO: models depend on materials?
		break;
	case AssetType::Texture:
		// Sprites use Textures
		for (auto const sprite : get_by_type<Sprite>())
		{
			if (sprite->get_texture() == asset)
			{
				result.push_back(sprite);
			}
		}
		// FontVariants use Textures and Material Templates
		for (auto const font : get_by_type<FontVariant>())
		{
			if (font->get_texture() == asset)
			{
				result.push_back(font->get_texture());
			}
			else if (font->get_material().get() && (font->get_material()->get_template() == asset))
			{
				result.push_back(font->get_material()->get_template());
			}
		}
		break;
	case AssetType::FontVariant:
		// Fonts use FontVariants
		for (auto const font : get_by_type<Font>())
		{
			for (Ref<FontVariant> const variant : font->get_variants())
			{
				if (variant == asset)
				{
					result.push_back(variant);
				}
			}
		}
	}

	return result;
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

Ref<Animation> Minty::AssetManager::load_animation(Path const& path)
{
	MINTY_ASSERT_ASSET_PATH(path);

	AnimationBuilder builder{};
	builder.id = read_id(path);

	Container* container;
	Reader* reader;
	if (open_reader(path, container, reader))
	{
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
	}

	return create_existing<Animation>(path, builder);
}

Ref<Animator> Minty::AssetManager::load_animator(Path const& path)
{
	MINTY_ASSERT_ASSET_PATH(path);

	AnimatorBuilder builder{};
	builder.id = read_id(path);

	Container* container;
	Reader* reader;

	if (open_reader(path, container, reader))
	{
		// basically just read the FSM
		reader->read("fsm", builder.fsm);

		close_reader(container, reader);
	}

	return create_existing<Animator>(path, builder);
}

Ref<Font> Minty::AssetManager::load_font(Path const& path)
{
	// read font file
	MINTY_ASSERT_ASSET_PATH(path);

	FontBuilder builder{};
	builder.id = read_id(path);

	Container* container;
	Reader* reader;
	if (open_reader(path, container, reader))
	{
		reader->read("Name", builder.name);

		// read variants
		std::vector<UUID> variantIds;
		if (reader->read("Variants", variantIds))
		{
			for (UUID const id : variantIds)
			{
				if (!contains(id))
				{
					MINTY_ERROR_FORMAT("Unable to find FontVariant with id \"{}\".", to_string(id));
					continue;
				}

				builder.variants.push_back(get<FontVariant>(id));
			}
		}

		close_reader(container, reader);
	}

	return create_existing<Font>(path, builder);
}

Ref<FontVariant> Minty::AssetManager::load_font_variant(Path const& path)
{
	MINTY_ASSERT_ASSET_PATH(path);

	FontVariantBuilder builder{};

	// open meta as normal file
	Path metaPath = Asset::get_meta_path(path);

	Container* container = nullptr;
	Reader* reader = nullptr;
	if (open_reader(metaPath, container, reader))
	{
		// read ID
		Node const& root = reader->get_current_node();
		String rootDataString = root.get_data_as_string();
		builder.id = Parse::to_uuid(rootDataString);

		close_reader(container, reader);
	}

	// read fnt file
	std::vector<String> lines = read_file_lines(path);
	Float widthScale = 1.0f;
	Float heightScale = 1.0f;
	for (String const& line : lines)
	{
		// split by tabs
		std::vector<String> parts = split_words(line);

		// determine what to do based on first word in line
		if (line.starts_with("char "))
		{
			FontChar fontChar{};

			for (String const& part : parts)
			{
				if (part.starts_with("id="))
				{
					fontChar.id = Parse::to_char(part.substr(3, part.length() - 3));
				}
				else if (part.starts_with("x="))
				{
					fontChar.x = Parse::to_int(part.substr(2, part.length() - 2)) * widthScale;
				}
				else if (part.starts_with("y="))
				{
					fontChar.y = Parse::to_int(part.substr(2, part.length() - 2)) * heightScale;
				}
				else if (part.starts_with("width="))
				{
					fontChar.width = Parse::to_int(part.substr(6, part.length() - 6)) * widthScale;
				}
				else if (part.starts_with("height="))
				{
					fontChar.height = Parse::to_int(part.substr(7, part.length() - 7)) * heightScale;
				}
				else if (part.starts_with("xoffset="))
				{
					fontChar.xOffset = Parse::to_int(part.substr(8, part.length() - 8)) * widthScale;
				}
				else if (part.starts_with("yoffset="))
				{
					fontChar.yOffset = Parse::to_int(part.substr(8, part.length() - 8)) * heightScale;
				}
				else if (part.starts_with("xadvance="))
				{
					fontChar.xAdvance = Parse::to_int(part.substr(9, part.length() - 9)) * widthScale;
				}
			}

			builder.characters.push_back(fontChar);
		}
		else if (line.starts_with("kerning "))
		{
			int first = 0;
			int second = 0;
			float amount = 0;
			for (String const& part : parts)
			{
				if (part.starts_with("first="))
				{
					first = Parse::to_int(part.substr(6, part.length() - 6));
				}
				else if (part.starts_with("second="))
				{
					second = Parse::to_int(part.substr(7, part.length() - 7));
				}
				else if (part.starts_with("amount="))
				{
					amount = Parse::to_int(part.substr(7, part.length() - 7)) * widthScale;
				}
			}

			// pack kerning into builder
			builder.kernings.emplace(FontVariant::compact_kerning(first, second), amount);
		}
		else if (line.starts_with("info "))
		{
			for (String const& part : parts)
			{
				if (part.starts_with("size="))
				{
					builder.size = Parse::to_uint(part.substr(5, part.length() - 5));
				}
				else if (part.starts_with("bold="))
				{
					builder.bold = static_cast<bool>(Parse::to_int(part.substr(5, part.length() - 5)));
				}
				else if (part.starts_with("italic="))
				{
					builder.italic = static_cast<bool>(Parse::to_int(part.substr(7, part.length() - 7)));
				}
			}
		}
		else if (line.starts_with("common "))
		{
			for (String const& part : parts)
			{
				if (part.starts_with("lineHeight="))
				{
					builder.lineHeight = static_cast<float>(Parse::to_int(part.substr(11, part.length() - 11)));
				}
			}
		}
		else if (line.starts_with("page "))
		{
			// textures to load
			Path directoryPath = path.parent_path();
			std::vector<void*> dependencyTextures;

			for (String const& part : parts)
			{
				if (part.starts_with("file="))
				{
					// ignore " "
					String name = part.substr(6, part.length() - 7);
					UUID textureId = read_id(directoryPath / name);
					builder.texture = get<Texture>(textureId);
					dependencyTextures.push_back(builder.texture.get());
				}
			}

			//CHECK_DEPENDENCIES(path.stem().string(), dependencyTextures);

			if (builder.texture == nullptr || !builder.texture->get_width() || !builder.texture->get_height())
			{
				break;
			}

			widthScale = 1.0f / builder.texture->get_width();
			heightScale = 1.0f / builder.texture->get_height();
		}
	}

	return create_existing<FontVariant>(path, builder);
}

Owner<Image> Minty::AssetManager::load_image(Path const& path)
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

		MINTY_ASSERT_FORMAT(shader->has_input(inputName), "Shader does not have an input with the name \"{}\". Shader: \"{}\". Asset: \"{}\".", inputName, get_path(shader->id()).generic_string(), path.generic_string());

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
			switch (slot.type)
			{
			case Type::Bool:
				reader.read_to_container<Bool>(j, slot.container);
				break;
			case Type::Bool2:
				reader.read_to_container<Bool2>(j, slot.container);
				break;
			case Type::Bool3:
				reader.read_to_container<Bool3>(j, slot.container);
				break;
			case Type::Bool4:
				reader.read_to_container<Bool4>(j, slot.container);
				break;
			case Type::Char:
				reader.read_to_container<Char>(j, slot.container);
				break;
			case Type::Byte:
				reader.read_to_container<Byte>(j, slot.container);
				break;
			case Type::Short:
				reader.read_to_container<Short>(j, slot.container);
				break;
			case Type::UShort:
				reader.read_to_container<UShort>(j, slot.container);
				break;
			case Type::Int:
				reader.read_to_container<Int>(j, slot.container);
				break;
			case Type::Int2:
				reader.read_to_container<Int2>(j, slot.container);
				break;
			case Type::Int3:
				reader.read_to_container<Int3>(j, slot.container);
				break;
			case Type::Int4:
				reader.read_to_container<Int4>(j, slot.container);
				break;
			case Type::UInt:
				reader.read_to_container<UInt>(j, slot.container);
				break;
			case Type::UInt2:
				reader.read_to_container<UInt2>(j, slot.container);
				break;
			case Type::UInt3:
				reader.read_to_container<UInt3>(j, slot.container);
				break;
			case Type::UInt4:
				reader.read_to_container<UInt4>(j, slot.container);
				break;
			case Type::Long:
				reader.read_to_container<Long>(j, slot.container);
				break;
			case Type::ULong:
				reader.read_to_container<ULong>(j, slot.container);
				break;
			case Type::Size:
				reader.read_to_container<Size>(j, slot.container);
				break;
			case Type::Float:
				reader.read_to_container<Float>(j, slot.container);
				break;
			case Type::Float2:
				reader.read_to_container<Float2>(j, slot.container);
				break;
			case Type::Float3:
				reader.read_to_container<Float3>(j, slot.container);
				break;
			case Type::Float4:
				reader.read_to_container<Float4>(j, slot.container);
				break;
			case Type::Double:
				reader.read_to_container<Double>(j, slot.container);
				break;
			case Type::Matrix2:
				reader.read_to_container<Matrix2>(j, slot.container);
				break;
			case Type::Matrix3:
				reader.read_to_container<Matrix3>(j, slot.container);
				break;
			case Type::Matrix4:
				reader.read_to_container<Matrix4>(j, slot.container);
				break;
			case Type::Quaternion:
				reader.read_to_container<Quaternion>(j, slot.container);
				break;
			case Type::Asset:
			{
				// read ID, then get asset ref, copy pointer to container
				Ref<Asset> asset{};
				if (!find_dependency(path, reader, valueName, asset, true))
				{
					return false;
				}
				Asset* assetPtr = asset.get();
				slot.container.set(&assetPtr, sizeof(assetPtr));
				break;
			}
			case Type::UUID:
				reader.read_to_container<UUID>(j, slot.container);
				break;
			default:
				MINTY_ABORT_FORMAT("Failed to load value with type \"{}\". Functionality not yet implemented.", to_string(slot.type));
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
		if (!find_dependency<MaterialTemplate>(path, *reader, "Template", builder.materialTemplate, true))
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
		if (!find_dependency<Shader>(path, *reader, "Shader", builder.shader, true))
		{
			close_reader(container, reader);
			return Ref<MaterialTemplate>();
		}

		if (reader->indent("Values"))
		{
			if (!load_values(*reader, builder.values, builder.shader, path))
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

Ref<Shader> Minty::AssetManager::load_shader(Path const& path)
{
	MINTY_ASSERT_ASSET_PATH(path);

	ShaderBuilder builder{};
	builder.id = read_id(path);

	Container* container;
	Reader* reader;
	if (open_reader(path, container, reader))
	{

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

						// emplace slot
						input.cargo.emplace_slot(name, type);

						// add to total size
						UInt typeSize = static_cast<UInt>(sizeof_type(type));
						input.size += typeSize;
					}

					// step out of Structure
					reader->outdent();
				}

				// adjust offset if push const, so next push is const aligned
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

			String name;
			UInt binding = UINT_MAX;
			UInt location;
			for (Size i = 0; i < reader->size(); i++)
			{
				ShaderBinding& shaderBinding = builder.vertexInput.bindings.at(i);

				// read binding
				if (!reader->read_name(i, name) || !Parse::try_uint(name, binding))
				{
					binding += 1;
				}
				shaderBinding.binding = binding;

				// read rate
				reader->read(i, shaderBinding.rate);

				// read attributes
				reader->indent(i);

				// allot space for each attribute
				shaderBinding.attributes.resize(reader->size());

				// read each attribute
				location = UINT_MAX;
				for (Size j = 0; j < reader->size(); j++)
				{
					ShaderAttribute& shaderAttribute = shaderBinding.attributes.at(j);

					// get attribute location
					if (!reader->read_name(j, name) || !Parse::try_uint(name, location))
					{
						// set location to last location + 1
						location += 1;
					}
					shaderAttribute.location = location;

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
			if (!find_dependency<ShaderModule>(path, *reader, "Vertex", builder.vertexShaderModule, true))
			{
				close_reader(container, reader);
				return Ref<Shader>();
			}
			if (!find_dependency<ShaderModule>(path, *reader, "Fragment", builder.fragmentShaderModule, true))
			{
				close_reader(container, reader);
				return Ref<Shader>();
			}

			reader->outdent();
		}

		// viewport and scissor
		if (!find_dependency<Viewport>(path, *reader, "Viewport", builder.viewport, false))
		{
			// default
			builder.viewport = Renderer::get_viewport();
		}
		if (!find_dependency<Scissor>(path, *reader, "Scissor", builder.scissor, false))
		{
			// default
			builder.scissor = Renderer::get_scissor();
		}

		close_reader(container, reader);
	}

	return create_existing<Shader>(path, builder);
}

Ref<ShaderModule> Minty::AssetManager::load_shader_module(Path const& path)
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
	if (open_reader(path, container, reader))
	{
		if (!find_dependency(path, *reader, "Texture", builder.texture, false))
		{
			// default
			builder.texture = get<Texture>(DEFAULT_TEXTURE);

			MINTY_ASSERT_MESSAGE(builder.texture != nullptr, "Default Texture not loaded.");
		}
		reader->read("CoordinateMode", builder.coordinateMode);
		reader->read("Offset", builder.offset);
		reader->read("Size", builder.size);
		reader->read("Pivot", builder.pivot);
		reader->read("PPU", builder.pixelsPerUnit);

		close_reader(container, reader);
	}

	return create_existing<Sprite>(path, builder);
}

Ref<Texture> Minty::AssetManager::load_texture(Path const& path)
{
	MINTY_ASSERT_ASSET_PATH(path);

	TextureBuilder builder{};
	builder.id = read_id(path);
	builder.image = load_image(path);

	return create_existing<Texture>(path, builder);
}
