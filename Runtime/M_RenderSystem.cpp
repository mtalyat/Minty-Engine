#include "pch.h"
#include "M_RenderSystem.h"

#include "M_Asset.h"
#include "M_Engine.h"
#include "M_Builtin.h"
#include "M_Vector.h"
#include "glm.hpp"

#include "M_Rendering_DrawCallObjectInfo.h"

using namespace minty;
using namespace minty::rendering;
using namespace minty::vk;
using namespace minty::builtin;

minty::RenderSystem::RenderSystem(Scene& scene)
	: System::System(scene)
	, _renderEngine(&scene.get_engine().get_render_engine())
{}

minty::RenderSystem::~RenderSystem()
{}

void minty::RenderSystem::reset()
{
	
}

void minty::RenderSystem::update()
{
	// do nothing if no camera
	if (_mainCamera == NULL_ENTITY)
	{
		console::warn("There is no Camera to render to!");
		return;
	}

	// get camera transform
	EntityRegistry& entityRegistry = get_entity_registry();
	CameraComponent const& camera = entityRegistry.get<CameraComponent>(_mainCamera);
	TransformComponent const& transformComponent = entityRegistry.get<TransformComponent>(_mainCamera);

	// update camera in renderer
	update_camera(camera, transformComponent);
}

void minty::RenderSystem::unload()
{
	// destroy assets
	destroy_assets();
}

void minty::RenderSystem::update_camera(CameraComponent const& camera, TransformComponent const& transform)
{
	Vector4 matPos = transform.global[3];
	Vector3 globalPos = Vector3(matPos.x, matPos.y, matPos.z);

	Matrix4 view = glm::lookAt(globalPos, globalPos + transform.local.rotation.forward(), Vector3(0.0f, 1.0f, 0.0f));

	// TODO: don't use lookat
	// maybe invert global?

	// get projection
	Matrix4 proj;
	switch (camera.perspective)
	{
	case CameraComponent::Perspective::Perspective:
		proj = glm::perspective(glm::radians(camera.fov), _renderEngine->get_aspect_ratio(), camera.nearPlane, camera.farPlane);
		break;
	default:
		proj = Matrix4(1.0f);
		break;
	}

	// multiply together
	Matrix4 transformMatrix = proj * view;

	// update buffer object
	CameraBufferObject obj =
	{
		.transform = transformMatrix,
	};

	// update all shaders
	for (auto& shader : _shaders)
	{
		shader.second.update_global_uniform_constant("camera", _renderEngine->get_frame(), &obj, sizeof(CameraBufferObject), 0);
	}
}

ID minty::RenderSystem::create_texture(rendering::TextureBuilder const& builder)
{
	return _textures.emplace(builder.name, Texture(builder, *_renderEngine));
}

ID minty::RenderSystem::create_sprite(rendering::SpriteBuilder const& builder)
{
	return _sprites.emplace(builder.name, Sprite(builder, *_renderEngine));
}

ID minty::RenderSystem::create_shader(rendering::ShaderBuilder const& builder)
{
	return _shaders.emplace(builder.name, Shader(builder, *_renderEngine));
}

ID minty::RenderSystem::create_shader_pass(rendering::ShaderPassBuilder const& builder)
{
	return _shaderPasses.emplace(builder.name, ShaderPass(builder, *_renderEngine));
}

ID minty::RenderSystem::create_material_template(rendering::MaterialTemplateBuilder const& builder)
{
	return _materialTemplates.emplace(builder.name, MaterialTemplate(builder, *_renderEngine));
}

ID minty::RenderSystem::create_material(rendering::MaterialBuilder const& builder)
{
	return _materials.emplace(builder.name, Material(builder, *_renderEngine));
}

ID minty::RenderSystem::create_mesh()
{
	// just create a brand new mesh
	return _meshes.emplace(Mesh(*_renderEngine));
}

ID minty::RenderSystem::get_or_create_mesh(String const& name)
{
	// if contains name, return it
	if (_meshes.contains(name))
	{
		return _meshes.get_id(name);
	}

	// create new
	return _meshes.emplace(name, Mesh(*_renderEngine));
}

ID minty::RenderSystem::get_or_create_mesh(MeshType const type)
{
	String name = to_string(type);

	// if contains type, return that
	if (_meshes.contains(name))
	{
		return _meshes.get_id(name);
	}

	// create new
	ID id = _meshes.emplace(name, Mesh(*_renderEngine));
	Mesh& mesh = _meshes.at(id);

	switch (type)
	{
	case MeshType::Custom:
		// do nothing for now
		console::todo("MeshType::Custom");
		break;
	case MeshType::Quad:
		Mesh::create_primitive_quad(mesh);
		break;
	case MeshType::Cube:
		Mesh::create_primitive_cube(mesh);
		break;
	case MeshType::Pyramid:
		Mesh::create_primitive_pyramid(mesh);
		break;
	case MeshType::Sphere:
		Mesh::create_primitive_sphere(mesh);
		break;
	case MeshType::Cylinder:
		Mesh::create_primitive_cylinder(mesh);
		break;
	default:
		// empty mesh
		mesh.clear();
		break;
	}

	return id;
}

ID minty::RenderSystem::find_texture(String const& name) const
{
	if (!_textures.contains(name))
	{
		return ERROR_ID;
	}

	return _textures.get_id(name);
}

ID minty::RenderSystem::find_sprite(String const& name) const
{
	if (!_sprites.contains(name))
	{
		return ERROR_ID;
	}

	return _sprites.get_id(name);
}

ID minty::RenderSystem::find_shader(String const& name) const
{
	if (!_shaders.contains(name))
	{
		return ERROR_ID;
	}

	return _shaders.get_id(name);
}

ID minty::RenderSystem::find_shader_pass(String const& name) const
{
	if (!_shaderPasses.contains(name))
	{
		return ERROR_ID;
	}

	return _shaderPasses.get_id(name);
}

ID minty::RenderSystem::find_material_template(String const& name) const
{
	if (!_materialTemplates.contains(name))
	{
		return ERROR_ID;
	}

	return _materialTemplates.get_id(name);
}

ID minty::RenderSystem::find_material(String const& name) const
{
	if (!_materials.contains(name))
	{
		return ERROR_ID;
	}

	return _materials.get_id(name);
}

ID minty::RenderSystem::find_mesh(String const& name) const
{
	if (!_meshes.contains(name))
	{
		return ERROR_ID;
	}

	return _meshes.get_id(name);
}

String minty::RenderSystem::get_texture_name(ID const id) const
{
	return _textures.get_name(id);
}

String minty::RenderSystem::get_sprite_name(ID const id) const
{
	return _sprites.get_name(id);
}

String minty::RenderSystem::get_shader_name(ID const id) const
{
	return _shaders.get_name(id);
}

String minty::RenderSystem::get_shader_pass_name(ID const id) const
{
	return _shaderPasses.get_name(id);
}

String minty::RenderSystem::get_material_template_name(ID const id) const
{
	return _materialTemplates.get_name(id);
}

String minty::RenderSystem::get_material_name(ID const id) const
{
	return _materials.get_name(id);
}

String minty::RenderSystem::get_mesh_name(ID const id) const
{
	return _meshes.get_name(id);
}

ID minty::RenderSystem::load_texture(Path const& path)
{
	return load_texture(path, path.stem().string());
}

ID minty::RenderSystem::load_texture(Path const& path, String const& name)
{
	if (Asset::check(path, false))
	{
		return ERROR_ID;
	}

	Node meta = Asset::load_meta(path);
	Reader reader(meta);

	// create texture builder from path and meta file
	TextureBuilder builder
	{
		.name = name,
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

	return create_texture(builder);
}

ID minty::RenderSystem::load_sprite(Path const& path)
{
	return load_sprite(path, path.stem().string());
}

ID minty::RenderSystem::load_sprite(Path const& path, String const& name)
{
	if (Asset::check(path, false))
	{
		return ERROR_ID;
	}

	Node meta = Asset::load_node(path);
	Reader reader(meta);

	SpriteBuilder builder
	{
		.name = name,
		.textureId = find_texture(reader.read_string("texture")),
		.materialId = find_material(reader.read_string("material")),
		.coordinateMode = from_string_pixel_coordinate_mode(reader.read_string("coordinateMode")),
		.minCoords = Vector2(0.0f, 0.0f),
		.maxCoords = Vector2(1.0f, 1.0f),
		.pivot = Vector2(0.5f, 0.5f),
	};

	reader.read_object("min", builder.minCoords);
	reader.read_object("max", builder.maxCoords);
	reader.read_object("pivot", builder.pivot);

	return create_sprite(builder);
}

ID minty::RenderSystem::load_shader(Path const& path)
{
	return load_shader(path, path.stem().string());
}

ID minty::RenderSystem::load_shader(Path const& path, String const& name)
{
	if (Asset::check(path, false))
	{
		return ERROR_ID;
	}

	Node meta = Asset::load_node(path);

	ShaderBuilder builder;
	builder.name = path.stem().string();

	std::vector<Node> const* nodes;
	if (nodes = meta.find_all("push"))
	{
		for (Node const& child : *nodes)
		{
			PushConstantInfo pushConstantInfo;
			Reader reader(child);

			pushConstantInfo.name = reader.read_string("name", child.to_string());
			pushConstantInfo.stageFlags = from_string_vk_shader_stage_flag_bits(reader.read_string("stageFlags"));
			pushConstantInfo.offset = reader.read_uint("offset");
			pushConstantInfo.size = reader.read_uint("size");

			builder.pushConstantInfos.emplace(pushConstantInfo.name, pushConstantInfo);
		}
	}

	if (nodes = meta.find_all("uniform"))
	{
		for (Node const& child : *nodes)
		{
			UniformConstantInfo uniformConstantInfo;
			Reader reader(child);

			uniformConstantInfo.name = reader.read_string("name", child.to_string());
			uniformConstantInfo.type = from_string_vk_descriptor_type(reader.read_string("type"));
			uniformConstantInfo.set = reader.read_uint("set");
			uniformConstantInfo.binding = reader.read_uint("binding");
			uniformConstantInfo.count = reader.read_uint("count", 1u);
			uniformConstantInfo.size = static_cast<VkDeviceSize>(reader.read_size("size"));
			uniformConstantInfo.stageFlags = from_string_vk_shader_stage_flag_bits(reader.read_string("stageFlags"));

			builder.uniformConstantInfos.emplace(uniformConstantInfo.name, uniformConstantInfo);
		}
	}

	return create_shader(builder);
}

ID minty::RenderSystem::load_shader_pass(Path const& path)
{
	return load_shader_pass(path, path.stem().string());
}

ID minty::RenderSystem::load_shader_pass(Path const& path, String const& name)
{
	if (Asset::check(path, false))
	{
		return ERROR_ID;
	}

	Node meta = Asset::load_node(path);
	Reader reader(meta);

	ShaderPassBuilder builder;
	builder.name = path.stem().string();
	builder.shaderId = find_shader(reader.read_string("shader", meta.to_string()));
	builder.topology = from_string_vk_primitive_topology(reader.read_string("primitiveTopology"));
	builder.polygonMode = from_string_vk_polygon_mode(reader.read_string("polygonMode"));
	builder.cullMode = from_string_vk_cull_mode_flag_bits(reader.read_string("cullMode"));
	builder.frontFace = from_string_vk_front_face(reader.read_string("frontFace"));
	builder.lineWidth = reader.read_float("lineWidth", 1.0f);

	std::vector<Node> const* nodes;
	if (nodes = meta.find_all("binding"))
	{
		for (auto const& child : *nodes)
		{
			VkVertexInputBindingDescription binding = {};
			Reader childReader(child);

			binding.binding = childReader.read_uint("binding", child.to_uint());
			binding.stride = childReader.read_uint("stride");
			binding.inputRate = from_string_vk_vertex_input_rate(childReader.read_string("inputRate"));

			builder.vertexBindings.push_back(binding);
		}
	}
	if (nodes = meta.find_all("attribute"))
	{
		for (auto const& child : *nodes)
		{
			VkVertexInputAttributeDescription attribute = {};
			Reader childReader(child);

			attribute.location = childReader.read_uint("location", child.to_uint());
			attribute.binding = childReader.read_uint("binding");
			attribute.format = from_string_vk_format(childReader.read_string("format"));
			attribute.offset = childReader.read_uint("offset");

			builder.vertexAttributes.push_back(attribute);
		}
	}
	if (nodes = meta.find_all("stage"))
	{
		for (auto const& child : *nodes)
		{
			ShaderPassBuilder::ShaderStageInfo info;
			Reader childReader(child);

			info.stage = from_string_vk_shader_stage_flag_bits(childReader.read_string("stage", child.to_string()));
			String path = childReader.read_string("path");
			info.code = Asset::load_chars(path);
			info.entry = childReader.read_string("entry", "main");

			builder.stages.push_back(info);
		}
	}

	return create_shader_pass(builder);
}

ID minty::RenderSystem::load_material_template(Path const& path)
{
	return load_material_template(path, path.stem().string());
}

ID minty::RenderSystem::load_material_template(Path const& path, String const& name)
{
	if (Asset::check(path, false))
	{
		return ERROR_ID;
	}

	Node meta = Asset::load_node(path);

	MaterialTemplateBuilder builder;
	builder.name = path.stem().string();

	std::vector<Node> const* nodes;
	if (nodes = meta.find_all("pass"))
	{
		for (auto const& child : *nodes)
		{
			builder.shaderPassIds.push_back(find_shader_pass(child.to_string()));
		}
	}
	if (Node const* node = meta.find("defaults"))
	{
		// get shader uniform constant values so we know how to interpret the values in the materials
		ShaderPass const& shaderPass = get_shader_pass(builder.shaderPassIds.front());
		Shader const& shader = get_shader(shaderPass.get_shader_id());

		load_descriptor_values(builder.defaultValues, *node, shader.get_uniform_constant_infos(DESCRIPTOR_SET_MATERIAL));
	}

	return create_material_template(builder);
}

ID minty::RenderSystem::load_material(Path const& path)
{
	return load_material(path, path.stem().string());
}

ID minty::RenderSystem::load_material(Path const& path, String const& name)
{
	if (Asset::check(path, false))
	{
		return ERROR_ID;
	}

	Node meta = Asset::load_node(path);
	Reader reader(meta);

	MaterialBuilder builder;
	builder.name = Path(path).stem().string();

	builder.templateId = find_material_template(reader.read_string("template"));

	if (Node const* node = meta.find("values"))
	{
		// get shader uniform constant values so we know how to interpret the values in the materials
		MaterialTemplate const& materialTemplate = get_material_template(builder.templateId);
		ShaderPass const& shaderPass = get_shader_pass(materialTemplate.get_shader_pass_ids().front());
		Shader const& shader = get_shader(shaderPass.get_shader_id());

		load_descriptor_values(builder.values, *node, shader.get_uniform_constant_infos(DESCRIPTOR_SET_MATERIAL));
	}

	return create_material(builder);
}

ID minty::RenderSystem::load_mesh(Path const& path)
{
	return load_mesh(path, path.stem().string());
}

ID minty::RenderSystem::load_mesh(Path const& path, String const& name)
{
	if (Asset::check(path, false))
	{
		return ERROR_ID;
	}

	String extension = path.extension().string();

	if (extension != ".obj")
	{
		console::error(std::format("Cannot load_animation mesh from file type \"{}\".", extension));
		return ERROR_ID;
	}

	// override existing mesh with same name
	ID id = get_or_create_mesh(name);

	// determine how to load the file
	if (extension == ".obj")
	{
		load_mesh_obj(path, id);
	}

	return id;
}

void minty::RenderSystem::destroy_texture(ID const id)
{
	if (_textures.contains(id))
	{
		auto& value = _textures.at(id);
		value.destroy();
		_textures.erase(id);
	}
}

void minty::RenderSystem::destroy_sprite(ID const id)
{
	if (_sprites.contains(id))
	{
		auto& value = _sprites.at(id);
		value.destroy();
		_sprites.erase(id);
	}
}

void minty::RenderSystem::destroy_shader(ID const id)
{
	if (_shaders.contains(id))
	{
		auto& value = _shaders.at(id);
		value.destroy();
		_shaders.erase(id);
	}
}

void minty::RenderSystem::destroy_shader_pass(ID const id)
{
	if (_shaderPasses.contains(id))
	{
		auto& value = _shaderPasses.at(id);
		value.destroy();
		_shaderPasses.erase(id);
	}
}

void minty::RenderSystem::destroy_material_template(ID const id)
{
	if (_materialTemplates.contains(id))
	{
		MaterialTemplate& materialTemplate = _materialTemplates.at(id);
		materialTemplate.destroy();
		_materialTemplates.erase(id);
	}
}

void minty::RenderSystem::destroy_material(ID const id)
{
	if (_materials.contains(id))
	{
		Material& material = _materials.at(id);
		material.destroy();
		_materials.erase(id);
	}
}

void minty::RenderSystem::destroy_mesh(ID const id)
{
	if (_meshes.contains(id))
	{
		Mesh& mesh = _meshes.at(id);
		mesh.clear();
		_meshes.erase(id);
	}
}

void minty::RenderSystem::destroy_assets()
{
	for (auto& tex : _textures)
	{
		tex.second.destroy();
	}
	_textures.clear();
	for (auto& sprite : _sprites)
	{
		sprite.second.destroy();
	}
	_sprites.clear();
	for (auto& material : _materials)
	{
		material.second.destroy();
	}
	_materials.clear();
	for (auto& materialTemplate : _materialTemplates)
	{
		materialTemplate.second.destroy();
	}
	_materialTemplates.clear();
	for (auto& shaderPass : _shaderPasses)
	{
		shaderPass.second.destroy();
	}
	_shaderPasses.clear();
	for (auto& shader : _shaders)
	{
		shader.second.destroy();
	}
	_shaders.clear();
	for (auto& mesh : _meshes)
	{
		mesh.second.clear();
	}
	_meshes.clear();
}

Texture& minty::RenderSystem::get_texture(ID const id)
{
	return _textures.at(id);
}

Texture const& minty::RenderSystem::get_texture(ID const id) const
{
	return _textures.at(id);
}

Sprite& minty::RenderSystem::get_sprite(ID const id)
{
	return _sprites.at(id);
}

Sprite const& minty::RenderSystem::get_sprite(ID const id) const
{
	return _sprites.at(id);
}

Shader& minty::RenderSystem::get_shader(ID const id)
{
	return _shaders.at(id);
}

Shader const& minty::RenderSystem::get_shader(ID const id) const
{
	return _shaders.at(id);
}

Shader& minty::RenderSystem::get_shader_from_material_id(ID const id)
{
	return get_shader(get_shader_pass(get_material_template(get_material(id).get_template_id()).get_shader_pass_ids().front()).get_shader_id());
}

Shader const& minty::RenderSystem::get_shader_from_material_id(ID const id) const
{
	return get_shader(get_shader_pass(get_material_template(get_material(id).get_template_id()).get_shader_pass_ids().front()).get_shader_id());
}

ShaderPass& minty::RenderSystem::get_shader_pass(ID const id)
{
	return _shaderPasses.at(id);
}

ShaderPass const& minty::RenderSystem::get_shader_pass(ID const id) const
{
	return _shaderPasses.at(id);
}

ShaderPass& minty::RenderSystem::get_shader_pass_from_material_id(ID const id)
{
	return get_shader_pass(get_material_template(get_material(id).get_template_id()).get_shader_pass_ids().front());
}

ShaderPass const& minty::RenderSystem::get_shader_pass_from_material_id(ID const id) const
{
	return get_shader_pass(get_material_template(get_material(id).get_template_id()).get_shader_pass_ids().front());
}

MaterialTemplate& minty::RenderSystem::get_material_template(ID const id)
{
	return _materialTemplates.at(id);
}

MaterialTemplate const& minty::RenderSystem::get_material_template(ID const id) const
{
	return _materialTemplates.at(id);
}

MaterialTemplate& minty::RenderSystem::get_material_template_from_material_id(ID const id)
{
	return get_material_template(get_material(id).get_template_id());
}

MaterialTemplate const& minty::RenderSystem::get_material_template_from_material_id(ID const id) const
{
	return get_material_template(get_material(id).get_template_id());
}

Material& minty::RenderSystem::get_material(ID const id)
{
	return _materials.at(id);
}

Material const& minty::RenderSystem::get_material(ID const id) const
{
	return _materials.at(id);
}

Mesh& minty::RenderSystem::get_mesh(ID const id)
{
	return _meshes.at(id);
}

Mesh const& minty::RenderSystem::get_mesh(ID const id) const
{
	return _meshes.at(id);
}


void minty::RenderSystem::set_main_camera(Entity const entity)
{
	_mainCamera = entity;
}

void minty::RenderSystem::load_descriptor_values(std::unordered_map<String, Dynamic>& values, Node const& node, std::vector<rendering::UniformConstantInfo> const& infos) const
{
	// go through all the constant values
	for (auto const& info : infos)
	{
		// if the name exists in the node, interpret the values from the node
		auto const& found = node.children.find(info.name);
		if (found == node.children.end())
		{
			// not found in values, so skip it
			continue;
		}
#ifdef MINTY_DEBUG
		if (found->second.size() > 1)
		{
			console::warn(std::format("Duplicate descriptors named \"{}\" found. Only the first one is being used.", info.name));
		}
#endif
		Node const& child = found->second.front();

		switch (info.type)
		{
		case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
		{
			// create list of ids from given texture names
			size_t size = child.children.size();
			ID* ids = new ID[size];
			for (size_t i = 0; i < size; i++)
			{
				// if child with the index name exists, use that
				if (Node const* c = child.find(std::to_string(i)))
				{
					// should be a texture name, so load that into ids
					ids[i] = find_texture(c->to_string());
				}
				else
				{
					// if index name does not exist, show warning and set to ERROR_ID
					console::warn(std::format("Failed to load_animation texture with index {} into descriptor named \"{}\".", i, info.name));
					ids[i] = ERROR_ID;
				}
			}

			// add to values
			values.emplace(found->first, Dynamic(ids, sizeof(ID) * size));
			// done with original ids array
			delete[] ids;

			break;
		}
		case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
		{
			// read raw data from files directly into the dynamic, for now
			Dynamic d;
			Reader reader(child);
			d.deserialize(reader);
			values.emplace(found->first, d);
			break;
		}
		}
	}
}

void minty::RenderSystem::load_mesh_obj(Path const& path, ID const id)
{
	Node meta = Asset::load_meta(path);

	Mesh& mesh = get_mesh(id);

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
					faceIndices.x = parse::to_int(token) - 1;

					if (std::getline(setss, token, '/'))
					{
						faceIndices.y = parse::to_int(token) - 1;

						if (std::getline(setss, token, '/'))
						{
							faceIndices.z = parse::to_int(token) - 1;
						}
					}
				}

				// if combo exists, add that index
				auto const& found = faces.find(faceIndices);
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

void minty::RenderSystem::serialize(Writer& writer) const
{
	console::todo("RenderSystem::serialize()");
}

void minty::RenderSystem::deserialize(Reader const& reader)
{
	std::vector<Path> paths;
	reader.read_vector("textures", paths);
	for (Path const& path : paths)
		load_texture(path);
	reader.read_vector("shaders", paths);
	for (Path const& path : paths)
		load_shader(path);
	reader.read_vector("shaderPasses", paths);
	for (Path const& path : paths)
		load_shader_pass(path);
	reader.read_vector("materialTemplates", paths);
	for (Path const& path : paths)
		load_material_template(path);
	reader.read_vector("materials", paths);
	for (Path const& path : paths)
		load_material(path);
	reader.read_vector("meshes", paths);
	for (Path const& path : paths)
		load_mesh(path);
	reader.read_vector("sprites", paths);
	for (Path const& path : paths)
		load_sprite(path);

	// custom parse atlases
	Node const& node = reader.get_node();
	if (Node const* atlasesNode = node.find("atlases"))
	{
		TextureAtlasBuilder builder;
		TextureAtlas atlas(builder, *_renderEngine);

		Reader atlasesReader(*atlasesNode, reader.get_data());
		atlas.deserialize(atlasesReader);

		if (Node const* spritesNode = atlasesNode->find("sprites"))
		{
			// determine what to do with the sprites
			if (spritesNode->find("all"))
			{
				// create all sprites
				atlas.create_all();
			}
			else
			{
				// create specific sprites
				if (auto const* sprites = spritesNode->find_all("sprite"))
				{
					// create sprites at specific X Y coordinates
					for (Node const& n : *sprites)
					{
						Reader nReader(n, reader.get_data());
						int x = nReader.read_int("x");
						int y = nReader.read_int("y");
						Vector2 pivot;
						nReader.read_object("pivot", pivot, Vector2::half());
						CoordinateMode coordinateMode = from_string_pixel_coordinate_mode(nReader.read_string("coordinateMode"));

						atlas.create_sprite(x, y, pivot, coordinateMode);
					}
				}
				if (auto const* slices = spritesNode->find_all("slice"))
				{
					// create slices at specific locations
					for (Node const& n : *slices)
					{
						Reader nReader(n, reader.get_data());
						Vector2 minCoords, maxCoords;
						nReader.read_object("min", minCoords, Vector2::zero());
						nReader.read_object("max", maxCoords, Vector2::one());
						CoordinateMode coordinateMode = from_string_pixel_coordinate_mode(nReader.read_string("coordinateMode"));

						atlas.slice_sprite(minCoords, maxCoords, coordinateMode);
					}
				}
			}
		}
	}
}