#include "pch.h"
#include "M_AssetEngine.h"
#include "M_Asset.h"

#include "M_Reader.h"
#include "M_Writer.h"
#include "M_Parse.h"
#include "M_Audio.h"

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
{}

Texture* minty::AssetEngine::load_texture(Path const& path)
{
	if (Asset::check(path, nullptr, false))
	{
		return nullptr;
	}

	Node meta = Asset::load_meta(path);
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
	if (Asset::check(path, SPRITE_EXTENSION, false))
	{
		return nullptr;
	}

	Node meta = Asset::load_node(path);
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
	if (Asset::check(path, SHADER_EXTENSION, false))
	{
		return nullptr;
	}

	Node meta = Asset::load_node(path);

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
	if (Asset::check(path, SHADER_PASS_EXTENSION, false))
	{
		return nullptr;
	}

	Node meta = Asset::load_node(path);
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
		info.code = Asset::load_chars(path);
		info.entry = childReader.read_string("entry", "main");

		builder.stages.push_back(info);
	}

	return emplace_new(new ShaderPass(builder, get_runtime()));
}

MaterialTemplate* minty::AssetEngine::load_material_template(Path const& path)
{
	if (Asset::check(path, MATERIAL_TEMPLATE_EXTENSION, false))
	{
		return nullptr;
	}

	Node meta = Asset::load_node(path);

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
	if (Asset::check(path, MATERIAL_EXTENSION, false))
	{
		return nullptr;
	}

	Node meta = Asset::load_node(path);
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
	if (Asset::check(path, nullptr, false))
	{
		return nullptr;
	}

	String extension = path.extension().string();

	if (extension != ".obj")
	{
		Console::error(std::format("Cannot load_animation mesh from file type \"{}\".", extension));
		return nullptr;
	}

	Node meta = Asset::load_meta(path);

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
	std::vector<String> lines = Asset::load_lines(path);

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
	if (Asset::check(path, nullptr, false))
	{
		return nullptr;
	}

	// load meta
	Node meta = Asset::load_meta(path);
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
	if (Asset::check(path, ANIMATION_EXTENSION, false))
	{
		return nullptr;
	}

	Node meta = Asset::load_node(path);
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
	if (Asset::check(path, ANIMATOR_EXTENSION, false))
	{
		return nullptr;
	}

	Node meta = Asset::load_node(path);
	Reader reader(meta);

	AnimatorBuilder builder
	{
		.id = meta.to_uuid(),
		.path = path,
	};
	reader.read_object_ref("fsm", builder.fsm);

	return emplace_new(new Animator(builder, get_runtime()));
}

void minty::AssetEngine::unload(UUID const id)
{
	MINTY_ASSERT(_assets.contains(id));

	Asset* asset = _assets.at(id);

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
	for (auto const& pair : _assets)
	{
		delete pair.second;
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