#include "pch.h"
#include "M_AssetEngine.h"

#include "Minty/Assets/M_Asset.h"

#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"
#include "Minty/Tools/M_Parse.h"
#include "Minty/Audio/M_Audio.h"
#include "Minty/Files/M_VirtualFile.h"
#include "Minty/Files/M_PhysicalFile.h"
#include "Minty/Core/M_Application.h"
#include "Minty/Scripting/M_ScriptEngine.h"

// ASSETS:
#include "Minty/Rendering/M_Texture.h"
#include "Minty/Rendering/M_Sprite.h"
#include "Minty/Rendering/M_Shader.h"
#include "Minty/Rendering/M_ShaderPass.h"
#include "Minty/Rendering/M_MaterialTemplate.h"
#include "Minty/Rendering/M_Material.h"
#include "Minty/Rendering/M_Mesh.h"
#include "Minty/Rendering/M_Builtin.h"
#include "Minty/Audio/M_AudioClip.h"
#include "Minty/Animation/M_Animation.h"
#include "Minty/Animation/M_Animator.h"
#include "Minty/Assets/M_GenericAsset.h"

#ifdef MINTY_RELEASE
#define CHECK_MISSING_DEPENDENCIES(name, missing)
#define CHECK_DEPENDENCIES(name, ...)
#define CHECK(path)
#else
#define CHECK_MISSING_DEPENDENCIES(name, missing) if(missing) { MINTY_ERROR_FORMAT("Cannot load {}, missing {} dependencies.", name, missing); return nullptr; }
#define CHECK_DEPENDENCIES(name, ...) CHECK_MISSING_DEPENDENCIES(name, check_dependencies(std::vector<void*> { __VA_ARGS__ }))
#define CHECK(path) if(!check(path)) { return nullptr; }
#endif

using namespace Minty;
using namespace Minty::vk;
using namespace Minty::Builtin;

AssetEngine* Minty::AssetEngine::_instance = nullptr;

// util
void load_descriptor_values(AssetEngine& engine, std::unordered_map<String, Dynamic>& values, Node const& node, std::vector<UniformConstantInfo> const& infos)
{
	// go through all the constant values
	for (auto const& info : infos)
	{
		// if the name exists in the node, interpret the values from the node
		auto const* child = node.find(info.name);
		if (!child)
		{
			// not found in values, so skip it
			continue;
		}

		switch (info.type)
		{
		case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
		{
			// create list of ids from given texture names
			size_t size = child->get_children().size();
			UUID* ids = new UUID[size];
			for (size_t i = 0; i < size; i++)
			{
				// if child with the index name exists, use that
				if (Node const* c = child->find(std::to_string(i)))
				{
					// should be a texture name, so load that into ids
					ids[i] = c->to_uuid();
				}
				else
				{
					// if index name does not exist, show warning and set to ERROR_ID
					MINTY_WARN_FORMAT("Failed to load_animation texture with index {} into descriptor named \"{}\".", i, info.name);
					ids[i] = INVALID_UUID;
				}
			}

			// add to values
			values.emplace(child->get_name(), Dynamic(ids, sizeof(UUID) * size));
			// done with original ids array
			delete[] ids;

			break;
		}
		case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
		{
			// read raw data from files directly into the dynamic, for now
			Dynamic d;
			Reader reader(*child);
			d.deserialize(reader);
			values.emplace(child->get_name(), d);
			break;
		}
		}
	}
}

Minty::AssetEngine::AssetEngine()
	: Engine()
	, _assets()
	, _assetsByType()
	, _wrapper()
{
	MINTY_ASSERT_MESSAGE(!_instance, "There should only be one AssetEngine.");

	_instance = this;

	// by default, load any wrap files that are in the directory
	for (auto const& entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
	{
		if (Asset::check_type(entry.path(), AssetType::Wrap))
		{
			_wrapper.emplace(entry.path());

			// get wrap and print with version
			Wrap const& wrap = _wrapper.get_wrap(_wrapper.get_wrap_count() - 1);

			MINTY_LOG_FORMAT("Loading Wrap file \"{}\" v{}.", wrap.get_name(), wrap.get_content_version());
		}
	}
}

Minty::AssetEngine::~AssetEngine()
{
	unload_all();

	_instance = nullptr;
}

bool Minty::AssetEngine::exists(Path const& path) const
{
	switch (Application::instance().get_mode())
	{
	case ApplicationMode::Normal:
		return _wrapper.contains(path);
	case ApplicationMode::Edit:
		// wrapper could contain default assets
		return _wrapper.contains(path) || std::filesystem::exists(path);
	default:
		MINTY_ABORT("Unrecognized ApplicationMode.");
		return false;
	}
}

Ref<Asset> Minty::AssetEngine::load_asset(Path const& path)
{
	// get the asset type
	AssetType type = Asset::get_type(path);

	if (type == AssetType::None) return nullptr;

	// load based on the type
	switch (type)
	{
	case AssetType::Text:
		return load_generic(path);
	case AssetType::Script:
		return load_script(path);
	case AssetType::Texture:
		return load_texture(path);
	case AssetType::Sprite:
		return load_sprite(path);
	case AssetType::Material:
		return load_material(path);
	case AssetType::MaterialTemplate:
		return load_material_template(path);
	case AssetType::ShaderPass:
		return load_shader_pass(path);
	case AssetType::Shader:
		return load_shader(path);
	case AssetType::Mesh:
		return load_mesh(path);
	case AssetType::AudioClip:
		return load_audio_clip(path);
	case AssetType::Animation:
		return load_animation(path);
	case AssetType::Animator:
		return load_animator(path);
	default:
		MINTY_ERROR_FORMAT("load_asset not implemented for asset type: {}", path.extension().string());
		return nullptr;
	}
}

UUID Minty::AssetEngine::read_id(Path const& path)
{
	return read_file_meta(path).to_uuid();
}

bool Minty::AssetEngine::check(Path const& path) const
{
	// can load if assets exists, and if no meta is required, or if a meta is required, it exists
	// also needs to have the correct extension, if one was provided
	if (path.empty())
	{
		MINTY_ERROR("Asset path is empty.");
		return false;
	}
	else if (!exists(path))
	{
		MINTY_ERROR_FORMAT("Asset does not exist: \"{}\"", path.generic_string());
		return false;
	}
	else if (!exists(Asset::get_meta_path(path)))
	{
		MINTY_ERROR_FORMAT("Asset meta file does not exist: \"{}\"", Asset::get_meta_path(path).generic_string());
		return false;
	}

	return true;
}

int Minty::AssetEngine::check_dependencies(std::vector<void*> const& dependencies) const
{
	int missingDependencies = 0;

	for (auto const ptr : dependencies)
	{
		if (!ptr)
		{
			missingDependencies++;
		}
	}

	return missingDependencies;
}

Node Minty::AssetEngine::read_file_node(Path const& path) const
{
	// read file contents
	std::vector<String> lines = read_file_lines(path);

	// parse the lines
	return Node::parse(lines);
}

Node Minty::AssetEngine::read_file_meta(Path const& path) const
{
	return read_file_node(Asset::get_meta_path(path));
}

std::vector<char> Minty::AssetEngine::read_file(Path const& path) const
{
	switch (Application::instance().get_mode())
	{
	case ApplicationMode::Normal:
		return _wrapper.read(path);
	case ApplicationMode::Edit:
		// if wrapper contains the file, read from wrapper, otherwise read from disk
		if (_wrapper.contains(path)) return _wrapper.read(path);
		return File::read_all_chars(path);
	default:
		MINTY_ABORT("Unrecognized ApplicationMode.");
		return std::vector<char>();
	}
}

String Minty::AssetEngine::read_text(Path const& path) const
{
	// read file as chars
	std::vector<char> chars = read_file(path);

	// add null char for good measure
	chars.push_back('\0');

	// turn into string
	return String(chars.data());
}

std::vector<Byte> Minty::AssetEngine::read_file_bytes(Path const& path) const
{
	std::vector<char> data = read_file(path);

	std::vector<Byte> result(data.size());

	memcpy(result.data(), data.data(), sizeof(char) * data.size());

	return result;
}

std::vector<String> Minty::AssetEngine::read_file_lines(Path const& path) const
{
	std::vector<char> data = read_file(path);

	std::vector<String> result;

	std::istringstream ss(String(data.data(), sizeof(char) * data.size()));

	String line;
	while (std::getline(ss, line))
	{
		// remove the \r
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

		result.push_back(line);
	}

	return result;
}

Ref <Texture> Minty::AssetEngine::load_texture(Path const& path)
{
	CHECK(path);

	Node meta = read_file_meta(path);
	Reader reader(meta);

	// create texture builder from path and meta file
	TextureBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
		.pixelData = nullptr,
		.width = 0,
		.height = 0,
		.pixelFormat = from_string_texture_builder_pixel_format(reader.read_string("pixelFormat", "RGBA")),
		.filter = from_string_vk_filter(reader.read_string("filter", "NEAREST")),
		.format = from_string_vk_format(reader.read_string("format", "R8G8B8A8_SRGB")),
		.addressMode = from_string_vk_sampler_address_mode(reader.read_string("samplerAddressMode", "REPEAT")),
		.mipmapMode = from_string_vk_sampler_mipmap_mode(reader.read_string("samplerMipmapMode", "NEAREST")),
	};

	return create<Texture>(builder);
}

Ref <Sprite> Minty::AssetEngine::load_sprite(Path const& path)
{
	CHECK(path);

	Node node = read_file_node(path);
	Node meta = read_file_meta(path);
	Reader reader(node);

	SpriteBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
		.texture = get<Texture>(reader.read_uuid("texture")),
		.material = get<Material>(reader.read_uuid("material")),
		.coordinateMode = from_string_coordinate_mode(reader.read_string("coordinateMode")),
		.minCoords = reader.read_object("min", Vector2(0.0f, 0.0f)),
		.maxCoords = reader.read_object("max", Vector2(1.0f, 1.0f)),
		.pivot = reader.read_object("pivot", Vector2(0.5f, 0.5f)),
	};

	CHECK_DEPENDENCIES(path.filename().string(), builder.texture.get(), builder.material.get());

	return create<Sprite>(builder);
}

Ref <Shader> Minty::AssetEngine::load_shader(Path const& path)
{
	CHECK(path);

	Node node = read_file_node(path);
	Node meta = read_file_meta(path);

	ShaderBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
	};

	std::vector<Node const*> nodes = node.find_all("push");
	for (Node const* child : nodes)
	{
		PushConstantInfo pushConstantInfo;
		Reader reader(*child);

		pushConstantInfo.name = reader.read_string("name", reader.to_string());
		pushConstantInfo.stageFlags = from_string_vk_shader_stage_flag_bits(reader.read_string("stageFlags"));
		pushConstantInfo.offset = reader.read_uint("offset");
		pushConstantInfo.size = reader.read_uint("size");

		builder.pushConstantInfos.emplace(pushConstantInfo.name, pushConstantInfo);
	}

	nodes = node.find_all("uniform");
	for (Node const* child : nodes)
	{
		UniformConstantInfo uniformConstantInfo;
		Reader reader(*child);

		uniformConstantInfo.name = reader.read_string("name", reader.to_string());
		uniformConstantInfo.type = from_string_vk_descriptor_type(reader.read_string("type"));
		uniformConstantInfo.set = reader.read_uint("set");
		uniformConstantInfo.binding = reader.read_uint("binding");
		uniformConstantInfo.count = reader.read_uint("count", 1u);
		uniformConstantInfo.size = static_cast<VkDeviceSize>(reader.read_size("size"));
		uniformConstantInfo.stageFlags = from_string_vk_shader_stage_flag_bits(reader.read_string("stageFlags"));

		builder.uniformConstantInfos.emplace(uniformConstantInfo.name, uniformConstantInfo);
	}

	return create<Shader>(builder);
}

Ref <ShaderPass> Minty::AssetEngine::load_shader_pass(Path const& path)
{
	CHECK(path);

	Node node = read_file_node(path);
	Node meta = read_file_meta(path);
	Reader reader(node);

	ShaderPassBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
	};
	builder.shader = get<Shader>(reader.read_uuid("shader"));
	CHECK_DEPENDENCIES(path.filename().string(), builder.shader.get());

	builder.topology = from_string_vk_primitive_topology(reader.read_string("primitiveTopology"));
	builder.polygonMode = from_string_vk_polygon_mode(reader.read_string("polygonMode"));
	builder.cullMode = from_string_vk_cull_mode_flag_bits(reader.read_string("cullMode"));
	builder.frontFace = from_string_vk_front_face(reader.read_string("frontFace"));
	builder.lineWidth = reader.read_float("lineWidth", 1.0f);

	std::vector<Node const*> nodes = node.find_all("binding");
	for (auto const* child : nodes)
	{
		VkVertexInputBindingDescription binding = {};
		Reader childReader(*child);

		binding.binding = childReader.read_uint("binding", childReader.to_uint());
		binding.stride = childReader.read_uint("stride");
		binding.inputRate = from_string_vk_vertex_input_rate(childReader.read_string("inputRate"));

		builder.vertexBindings.push_back(binding);
	}
	nodes = node.find_all("attribute");
	for (auto const* child : nodes)
	{
		VkVertexInputAttributeDescription attribute = {};
		Reader childReader(*child);

		attribute.location = childReader.read_uint("location", childReader.to_uint());
		attribute.binding = childReader.read_uint("binding");
		attribute.format = from_string_vk_format(childReader.read_string("format"));
		attribute.offset = childReader.read_uint("offset");

		builder.vertexAttributes.push_back(attribute);
	}
	nodes = node.find_all("stage");
	for (auto const* child : nodes)
	{
		ShaderPassBuilder::ShaderStageInfo info;
		Reader childReader(*child);

		info.stage = from_string_vk_shader_stage_flag_bits(childReader.read_string("stage", childReader.to_string()));
		String path = childReader.read_string("path");
		info.code = read_file(path);
		info.entry = childReader.read_string("entry", "main");

		builder.stages.push_back(info);
	}

	return create<ShaderPass>(builder);
}

Ref <MaterialTemplate> Minty::AssetEngine::load_material_template(Path const& path)
{
	CHECK(path);

	Node node = read_file_node(path);
	Node meta = read_file_meta(path);

	MaterialTemplateBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
	};

	int missingPasses = 0;
	std::vector<Node const*> nodes = node.find_all("pass");
	for (auto const* child : nodes)
	{
		Ref<ShaderPass> pass = get<ShaderPass>(child->to_uuid());
		if (!pass) missingPasses++;
		else builder.shaderPasses.push_back(pass);
	}

	CHECK_MISSING_DEPENDENCIES(path.filename().string(), missingPasses);

	if (Node const* child = node.find("defaults"))
	{
		// get shader uniform constant values so we know how to interpret the values in the materials
		Ref<ShaderPass> shaderPass = builder.shaderPasses.front();
		Ref<Shader> shader = shaderPass->get_shader();

		load_descriptor_values(*this, builder.defaultValues, *child, shader->get_uniform_constant_infos(DESCRIPTOR_SET_MATERIAL));
	}

	return create<MaterialTemplate>(builder);
}

Ref <Material> Minty::AssetEngine::load_material(Path const& path)
{
	CHECK(path);

	Node node = read_file_node(path);
	Node meta = read_file_meta(path);
	Reader reader(node);

	MaterialBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
	};

	builder.materialTemplate = get<MaterialTemplate>(reader.read_uuid("template"));

	CHECK_DEPENDENCIES(path.filename().string(), builder.materialTemplate.get());

	if (Node const* child = node.find("values"))
	{
		// get shader uniform constant values so we know how to interpret the values in the materials
		MINTY_ASSERT(builder.materialTemplate != nullptr);
		Ref<ShaderPass> shaderPass = builder.materialTemplate->get_shader_passes().front();
		MINTY_ASSERT(shaderPass != nullptr);
		Ref<Shader> shader = shaderPass->get_shader();
		MINTY_ASSERT(shader != nullptr);

		load_descriptor_values(*this, builder.values, *child, shader->get_uniform_constant_infos(DESCRIPTOR_SET_MATERIAL));
	}

	return create<Material>(builder);
}

Ref <Mesh> Minty::AssetEngine::load_mesh(Path const& path)
{
	CHECK(path);

	String extension = path.extension().string();

	if (extension != ".obj")
	{
		MINTY_ERROR_FORMAT("Cannot load_animation mesh from file type \"{}\".", extension);
		return nullptr;
	}

	Node meta = read_file_meta(path);

	MeshBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
	};

	Ref<Mesh> mesh = create<Mesh>(builder);

	// determine how to load the file
	if (extension == ".obj")
	{
		load_mesh_obj(path, *mesh);
	}

	return mesh;
}

void Minty::AssetEngine::load_mesh_obj(Path const& path, Mesh& mesh)
{
	std::vector<String> lines = read_file_lines(path);

	std::vector<Vector3> positions;
	std::vector<Vector2> coords;
	std::vector<Vector3> normals;

	std::unordered_map<Vector3Int, uint16_t> faces;
	std::vector<Vertex3D> vertices;
	std::vector<uint16_t> indices;

	std::istringstream ss;
	String token;

	for (auto const& line : lines)
	{
		ss = std::istringstream(line);
		ss >> token;
		if (token == "v")
		{
			// position
			Vector3 position;
			ss >> position.x >> position.y >> position.z;
			position.y = -position.y; // flip Y
			positions.push_back(position);
		}
		else if (token == "vt")
		{
			// coord
			Vector2 coord;
			ss >> coord.x >> coord.y;
			coord.y = -coord.y; // flip y
			coords.push_back(coord);
		}
		else if (token == "vn")
		{
			// normal
			Vector3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (token == "f")
		{
			// face
			// get pairs
			for (size_t i = 0; i < 3; i++)
			{
				String set;
				ss >> set;

				std::istringstream setss(set);
				Vector3Int faceIndices = Vector3Int();

				// subtract 1, since all indices are 1 indexed apparently
				if (std::getline(setss, token, '/'))
				{
					faceIndices.x = Parse::to_int(token) - 1;

					if (std::getline(setss, token, '/'))
					{
						faceIndices.y = Parse::to_int(token) - 1;

						if (std::getline(setss, token, '/'))
						{
							faceIndices.z = Parse::to_int(token) - 1;
						}
					}
				}

				// if combo exists, add that index
				auto found = faces.find(faceIndices);
				if (found == faces.end())
				{
					// vertex does not exist yet

					uint16_t index = static_cast<uint16_t>(vertices.size());
					vertices.push_back(Vertex3D
						{
							.pos = positions[faceIndices.x],
							.normal = normals[faceIndices.z],
							.coord = coords[faceIndices.y]
						});
					indices.push_back(index);
					faces.emplace(faceIndices, index);
				}
				else
				{
					// vertex already exists

					uint16_t index = found->second;
					indices.push_back(index);
				}
			}
		}
	}

	// all vertices and indices populated
	mesh.set_vertices(vertices);
	mesh.set_indices(indices);
}

Ref <AudioClip> Minty::AssetEngine::load_audio_clip(Path const& path)
{
	CHECK(path);

	// load meta
	Node meta = read_file_meta(path);
	Reader reader(meta);

	AudioClipBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
		.volume = reader.read_float("volume", 1.0f),
		.looping = reader.read_bool("looping", false),
		.loopPoint = reader.read_float("loopingPoint", 0.0f),
		.singleInstance = reader.read_bool("singleInstance", false)
	};

	// the clip will load the audio file via the path in the constructor
	return create<AudioClip>(builder);
}

Ref <Animation> Minty::AssetEngine::load_animation(Path const& path)
{
	CHECK(path);

	Node node = read_file_node(path);
	Node meta = read_file_meta(path);
	Reader reader(node);

	// read basic values
	AnimationBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
		.length = reader.read_float("length"),
		.loops = reader.read_bool("loops"),
	};

	// read data vectors
	reader.read_vector("entities", builder.entities);
	reader.read_vector("components", builder.components);
	reader.read_vector("variables", builder.variables);
	if (Node const* valuesNode = reader.get_node().find("values"))
	{
		builder.values = valuesNode->get_children();
	}

	// read the steps, but split them up by '/'
	if (Node const* stepsNode = reader.get_node().find("steps"))
	{
		// for each child, that is a time stamp
		// each child of each time stamp are the actual steps
		// all are the node names
		for (Node const& timeStampNode : stepsNode->get_children())
		{
			float time = Parse::to_float(timeStampNode.get_name());

			// get actual step values
			for (Node const& stepNode : timeStampNode.get_children())
			{
				builder.steps.push_back(
					{
						time,
						Animation::parse_step(stepNode.get_node_string())
					});
			}
		}
	}
	if (Node const* resetsNode = reader.get_node().find("reset"))
	{
		for (Node const& resetNode : resetsNode->get_children())
		{
			builder.resetSteps.push_back(Animation::parse_step(resetNode.get_node_string()));
		}
	}

	return create<Animation>(builder);
}

Ref <Animator> Minty::AssetEngine::load_animator(Path const& path)
{
	CHECK(path);

	Node node = read_file_node(path);
	Node meta = read_file_meta(path);
	Reader reader(node);

	AnimatorBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
	};
	reader.read_object_ref("fsm", builder.fsm);

	return create<Animator>(builder);
}

Ref<Asset> Minty::AssetEngine::load_script(Path const& path)
{
	// load meta, get its id, that's it
	CHECK(path);

	Node meta = read_file_meta(path);

	// add to script engine for later use
	ScriptEngine& scriptEngine = ScriptEngine::instance();
	scriptEngine.register_script_id(meta.to_uuid(), path.stem().string());

	return create<Asset>(meta.to_uuid(), path);
}

Ref<GenericAsset> Minty::AssetEngine::load_generic(Path const& path)
{
	CHECK(path);

	Node meta = read_file_meta(path);

	std::vector<char> fileData = read_file(path);

	if (fileData.empty() || fileData.back() != '\0')
	{
		fileData.push_back('\0');
	}

	GenericAssetBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
		.text = String(fileData.data()),
	};

	return create<GenericAsset>(builder);
}

void Minty::AssetEngine::unload(UUID const id)
{
	auto found = _assets.find(id);

	if (found == _assets.end())
	{
		return;
	}

	// remove the reference
	erase(id);
}

void Minty::AssetEngine::unload(Asset const& asset)
{
	unload(asset.get_id());
}

void Minty::AssetEngine::unload_all()
{
	// get all IDs into a vector that can be iterated over
	std::vector<UUID> ids;
	ids.reserve(_assets.size());
	for (auto const& [uuid, asset] : _assets)
	{
		ids.push_back(uuid);
	}

	// iterate each id: erase and delete each asset
	Ref<Asset> asset;
	for (auto const id : ids)
	{
		if (asset = get_asset(id))
		{
			erase(id);
		}
	}

	_assets.clear();
	_assetsByType.clear();
}

std::vector<Ref<Asset>> Minty::AssetEngine::get_dependents(Ref<Asset> const asset) const
{
	std::vector<Ref<Asset>> result;

	// get type
	AssetType type = asset->get_type();

	// check based on the type
	// some types inherently have no dependents
	switch (type)
	{
	case AssetType::Shader:
		// ShaderPasses use Shaders
		for (auto const shaderPass : get_by_type<ShaderPass>())
		{
			if (shaderPass->get_shader() == asset)
			{
				// this shaderPass uses this shader
				result.push_back(shaderPass);
			}
		}
		break;
	case AssetType::ShaderPass:
		// MaterialTemplates use ShaderPasses
		for (auto const materialTemplate : get_by_type<MaterialTemplate>())
		{
			for (auto const shaderPass : materialTemplate->get_shader_passes())
			{
				if (shaderPass == asset)
				{
					result.push_back(materialTemplate);
				}
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
		// Sprites use Materials
		for (auto const sprite : get_by_type<Sprite>())
		{
			if (sprite->get_material() == asset)
			{
				result.push_back(sprite);
			}
		}
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
		break;
	}

	return result;
}

Ref<Asset> Minty::AssetEngine::get_asset(UUID const id) const
{
	auto found = _assets.find(id);

	if (found != _assets.end())
	{
		return found->second.create_ref();
	}

	return nullptr;
}

bool Minty::AssetEngine::contains(UUID const id) const
{
	return _assets.contains(id);
}

void Minty::AssetEngine::emplace(Owner<Asset> const asset)
{
	MINTY_ASSERT(asset != nullptr);
	MINTY_ASSERT_FORMAT(!_assets.contains(asset->get_id()), "An asset with the ID {} already exists and is loaded.", to_string(asset->get_id()));
	MINTY_ASSERT(asset->get_id().valid());

	_assets.emplace(asset->get_id(), asset);
	emplace_by_type(asset);
}

void Minty::AssetEngine::erase(UUID const id)
{
	MINTY_ASSERT(_assets.contains(id));

	erase_by_type(_assets.at(id));
	_assets.erase(id);
}

void Minty::AssetEngine::emplace_by_type(Ref<Asset> const asset)
{
	TypeID typeId = typeid(*asset.get());

	auto found = _assetsByType.find(typeId);

	if (found != _assetsByType.end())
	{
		// add to existing set
		found->second.emplace(asset);
	}
	else
	{
		// create new set
		_assetsByType.emplace(typeId, std::unordered_set<Ref<Asset>>());
		_assetsByType.at(typeId).emplace(asset);
	}
}

void Minty::AssetEngine::erase_by_type(Ref<Asset> const asset)
{
	TypeID typeId = typeid(*asset.get());

	auto found = _assetsByType.find(typeId);

	if (found != _assetsByType.end())
	{
		// erase from type set
		found->second.erase(asset);

		// remove type set if empty
		if (found->second.empty())
		{
			_assetsByType.erase(typeId);
		}
	}
}
