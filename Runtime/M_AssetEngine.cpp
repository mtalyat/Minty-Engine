#include "pch.h"
#include "M_AssetEngine.h"
#include "M_Asset.h"

#include "M_Reader.h"
#include "M_Writer.h"
#include "M_Parse.h"
#include "M_Audio.h"
#include "M_VirtualFile.h"
#include "M_PhysicalFile.h"
#include "M_Runtime.h"

// ASSETS:
#include "M_Texture.h"
#include "M_Sprite.h"
#include "M_Shader.h"
#include "M_ShaderPass.h"
#include "M_MaterialTemplate.h"
#include "M_Material.h"
#include "M_Mesh.h"
#include "M_Builtin.h"
#include "M_AudioClip.h"
#include "M_Animation.h"
#include "M_Animator.h"

using namespace minty;
using namespace minty::vk;
using namespace minty::Builtin;

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

minty::AssetEngine::AssetEngine(Runtime& runtime)
	: Engine(runtime)
	, _assets()
	, _assetsByType()
	, _wrapper()
{
	// by default, load any wrap files that are in the directory
	for (auto const& entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
	{
		if (Asset::check_type(entry.path(), AssetType::Wrap))
		{
			_wrapper.emplace(entry.path());
		}
	}
}

minty::AssetEngine::~AssetEngine()
{
	unload_all();
}

bool minty::AssetEngine::exists(Path const& path) const
{
	switch (get_runtime().get_mode())
	{
	case RunMode::Normal:
		return _wrapper.contains(path);
	case RunMode::Edit:
		return std::filesystem::exists(path);
	default:
		MINTY_ABORT("Unrecognized RunMode.");
		return false;
	}
}

Asset* minty::AssetEngine::load_asset(Path const& path)
{
	// get the asset type
	AssetType type = Asset::get_type(path);

	if (type == AssetType::None) return nullptr;

	// load based on the type
	switch (type)
	{
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
		return load_shader (path);
	case AssetType::Mesh:
		return load_mesh(path);
	case AssetType::AudioClip:
		return load_audio_clip(path);
	case AssetType::Animation:
		return load_animation(path);
	case AssetType::Animator:
		return load_animator(path);
	default:
		MINTY_ABORT(std::format("load_asset not implemented for asset type: {}", path.extension().string()));
		return nullptr;
	}
}

UUID minty::AssetEngine::load_id(Path const& path)
{
	Node node;

	// if no meta, load direct file
	// if meta, load that instead
	if (Asset::requires_meta(path.extension().string().c_str()))
	{
		node = read_file_meta(path);
	}
	else
	{
		node = read_file_node(path);
	}

	return node.to_uuid();
}

void minty::AssetEngine::check(Path const& path) const
{
	// can load if assets exists, and if no meta is required, or if a meta is required, it exists
	// also needs to have the correct extension, if one was provided
	MINTY_ASSERT(!path.empty());
	MINTY_ASSERT_FORMAT(exists(path), "Path does not exist: {}", std::filesystem::absolute(path).string());
	MINTY_ASSERT_FORMAT(!Asset::requires_meta(path) || exists(Asset::get_meta_path(path)), "Missing appropriate meta path for path: {}, missing meta path: {}", std::filesystem::absolute(path).string(), std::filesystem::absolute(Asset::get_meta_path(path)).string());
}

Node minty::AssetEngine::read_file_node(Path const& path) const
{
	// read file contents
	std::vector<String> lines = read_file_lines(path);

	// parse the lines
	return Node::parse(lines);
}

Node minty::AssetEngine::read_file_meta(Path const& path) const
{
	return read_file_node(Asset::get_meta_path(path));
}

std::vector<char> minty::AssetEngine::read_file(Path const& path) const
{
	switch (get_runtime().get_mode())
	{
	case RunMode::Normal:
		return _wrapper.read(path);
	case RunMode::Edit:
		return File::read_all_chars(path);
	default:
		MINTY_ABORT("Unrecognized RunMode.");
		return std::vector<char>();
	}
}

std::vector<Byte> minty::AssetEngine::read_file_bytes(Path const& path) const
{
	std::vector<char> data = read_file(path);

	std::vector<Byte> result(data.size());

	memcpy(result.data(), data.data(), sizeof(char) * data.size());

	return result;
}

std::vector<String> minty::AssetEngine::read_file_lines(Path const& path) const
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

Texture* minty::AssetEngine::load_texture(Path const& path)
{
	check(path);

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

	return emplace_new(new Texture(builder, get_runtime()));
}

Sprite* minty::AssetEngine::load_sprite(Path const& path)
{
	check(path);

	Node meta = read_file_node(path);
	Reader reader(meta);

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

	return emplace_new(new Sprite(builder, get_runtime()));
}

Shader* minty::AssetEngine::load_shader(Path const& path)
{
	check(path);

	Node meta = read_file_node(path);

	ShaderBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
	};

	std::vector<Node const*> nodes = meta.find_all("push");
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

	nodes = meta.find_all("uniform");
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

	return emplace_new(new Shader(builder, get_runtime()));
}

ShaderPass* minty::AssetEngine::load_shader_pass(Path const& path)
{
	check(path);

	Node meta = read_file_node(path);
	Reader reader(meta);

	ShaderPassBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
	};
	builder.shader = get<Shader>(reader.read_uuid("shader"));
	builder.topology = from_string_vk_primitive_topology(reader.read_string("primitiveTopology"));
	builder.polygonMode = from_string_vk_polygon_mode(reader.read_string("polygonMode"));
	builder.cullMode = from_string_vk_cull_mode_flag_bits(reader.read_string("cullMode"));
	builder.frontFace = from_string_vk_front_face(reader.read_string("frontFace"));
	builder.lineWidth = reader.read_float("lineWidth", 1.0f);

	std::vector<Node const*> nodes = meta.find_all("binding");
	for (auto const* child : nodes)
	{
		VkVertexInputBindingDescription binding = {};
		Reader childReader(*child);

		binding.binding = childReader.read_uint("binding", childReader.to_uint());
		binding.stride = childReader.read_uint("stride");
		binding.inputRate = from_string_vk_vertex_input_rate(childReader.read_string("inputRate"));

		builder.vertexBindings.push_back(binding);
	}
	nodes = meta.find_all("attribute");
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
	nodes = meta.find_all("stage");
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

	return emplace_new(new ShaderPass(builder, get_runtime()));
}

MaterialTemplate* minty::AssetEngine::load_material_template(Path const& path)
{
	check(path);

	Node meta = read_file_node(path);

	MaterialTemplateBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
	};

	std::vector<Node const*> nodes = meta.find_all("pass");
	for (auto const* child : nodes)
	{
		builder.shaderPasses.push_back(get<ShaderPass>(child->to_uuid()));
	}

	if (Node const* node = meta.find("defaults"))
	{
		// get shader uniform constant values so we know how to interpret the values in the materials
		ShaderPass const* shaderPass = builder.shaderPasses.front();
		Shader const* shader = shaderPass->get_shader();

		load_descriptor_values(*this, builder.defaultValues, *node, shader->get_uniform_constant_infos(DESCRIPTOR_SET_MATERIAL));
	}

	return emplace_new(new MaterialTemplate(builder, get_runtime()));
}

Material* minty::AssetEngine::load_material(Path const& path)
{
	check(path);

	Node meta = read_file_node(path);
	Reader reader(meta);

	MaterialBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
	};

	builder.materialTemplate = get<MaterialTemplate>(reader.read_uuid("template"));

	if (Node const* node = meta.find("values"))
	{
		// get shader uniform constant values so we know how to interpret the values in the materials
		MINTY_ASSERT(builder.materialTemplate != nullptr);
		ShaderPass const* shaderPass = builder.materialTemplate->get_shader_passes().front();
		MINTY_ASSERT(shaderPass != nullptr);
		Shader const* shader = shaderPass->get_shader();
		MINTY_ASSERT(shader != nullptr);

		load_descriptor_values(*this, builder.values, *node, shader->get_uniform_constant_infos(DESCRIPTOR_SET_MATERIAL));
	}

	return emplace_new(new Material(builder, get_runtime()));
}

Mesh* minty::AssetEngine::load_mesh(Path const& path)
{
	check(path);

	String extension = path.extension().string();

	if (extension != ".obj")
	{
		Console::error(std::format("Cannot load_animation mesh from file type \"{}\".", extension));
		return nullptr;
	}

	Node meta = read_file_meta(path);

	MeshBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
	};

	Mesh* mesh = new Mesh(builder, get_runtime());

	// determine how to load the file
	if (extension == ".obj")
	{
		load_mesh_obj(path, *mesh);
	}

	return emplace_new(mesh);
}

void minty::AssetEngine::load_mesh_obj(Path const& path, Mesh& mesh)
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

AudioClip* minty::AssetEngine::load_audio_clip(Path const& path)
{
	check(path);

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
	return emplace_new(new AudioClip(builder, get_runtime()));
}

Animation* minty::AssetEngine::load_animation(Path const& path)
{
	check(path);

	Node meta = read_file_node(path);
	Reader reader(meta);

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

	return emplace_new(new Animation(builder, get_runtime()));
}

Animator* minty::AssetEngine::load_animator(Path const& path)
{
	check(path);

	Node meta = read_file_node(path);
	Reader reader(meta);

	AnimatorBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
	};
	reader.read_object_ref("fsm", builder.fsm);

	return emplace_new(new Animator(builder, get_runtime()));
}

Asset* minty::AssetEngine::load_script(Path const& path)
{
	// load meta, get its id, that's it
	check(path);

	Node meta = read_file_meta(path);

	// add to script engine for later use
	ScriptEngine& scriptEngine = get_runtime().get_script_engine();
	scriptEngine.register_script_id(meta.to_uuid(), path.stem().string());
	
	return emplace_new(new Asset(meta.to_uuid(), path, get_runtime()));
}

void minty::AssetEngine::unload(UUID const id)
{
	auto found = _assets.find(id);

	if (found == _assets.end())
	{
		return;
	}

	Asset* asset = found->second;

	// remove the reference
	erase(id);

	// delete the data
	delete asset;
}

void minty::AssetEngine::unload(Asset const& asset)
{
	unload(asset.get_id());
}

void minty::AssetEngine::unload_all()
{
	// get all IDs into a vector that can be iterated over
	std::vector<UUID> ids;
	ids.reserve(_assets.size());
	for (auto const& [uuid, asset] : _assets)
	{
		ids.push_back(uuid);
	}

	// iterate each id: erase and delete each asset
	Asset* asset;
	for (auto const id : ids)
	{
		if (asset = get_asset(id))
		{
			erase(id);
			delete asset;
		}
	}

	_assets.clear();
	_assetsByType.clear();
}

Asset* minty::AssetEngine::get_asset(UUID const id) const
{
	auto found = _assets.find(id);

	if (found != _assets.end())
	{
		return found->second;
	}

	return nullptr;
}

bool minty::AssetEngine::contains(UUID const id) const
{
	return _assets.contains(id);
}

void minty::AssetEngine::emplace(Asset* const asset)
{
	MINTY_ASSERT(asset != nullptr);
	MINTY_ASSERT(!_assets.contains(asset->get_id()));
	MINTY_ASSERT(asset->get_id().valid());

	_assets.emplace(asset->get_id(), asset);
	emplace_by_type(*asset);
}

void minty::AssetEngine::erase(UUID const id)
{
	MINTY_ASSERT(_assets.contains(id));

	erase_by_type(*_assets.at(id));
	_assets.erase(id);
}

void minty::AssetEngine::emplace_by_type(Asset& asset)
{
	TypeID typeId = typeid(asset);

	auto found = _assetsByType.find(typeId);

	if (found != _assetsByType.end())
	{
		// add to existing set
		found->second.emplace(&asset);
	}
	else
	{
		// create new set
		_assetsByType.emplace(typeId, std::unordered_set<Asset*>());
		_assetsByType.at(typeId).emplace(&asset);
	}
}

void minty::AssetEngine::erase_by_type(Asset& asset)
{
	TypeID typeId = typeid(asset);

	auto found = _assetsByType.find(typeId);

	if (found != _assetsByType.end())
	{
		// erase from type set
		found->second.erase(&asset);

		// remove type set if empty
		if (found->second.empty())
		{
			_assetsByType.erase(typeId);
		}
	}
}
