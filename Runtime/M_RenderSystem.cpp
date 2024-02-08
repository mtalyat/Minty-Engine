#include "pch.h"
#include "M_RenderSystem.h"

#include "M_Asset.h"
#include "M_Engine.h"
#include "M_Builtin.h"
#include "M_Vector.h"
#include "glm.hpp"

#include "M_DrawCallObjectInfo.h"

using namespace minty;
using namespace minty;
using namespace minty::vk;
using namespace minty::Builtin;

minty::RenderSystem::RenderSystem(Engine& engine, ID const sceneId)
	: System::System(engine, sceneId)
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
		Console::warn("There is no Camera to render to!");
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
	Vector4 matPos = transform.globalMatrix[3];
	Vector3 globalPos = Vector3(matPos.x, matPos.y, matPos.z);

	Matrix4 view = glm::lookAt(globalPos, globalPos + transform.localRotation.forward(), Vector3(0.0f, 1.0f, 0.0f));

	// TODO: don't use lookat
	// maybe invert global?

	RenderEngine& renderEngine = get_engine().get_render_engine();

	// get projection
	Matrix4 proj;
	switch (camera.perspective)
	{
	case CameraComponent::Perspective::Perspective:
		proj = glm::perspective(glm::radians(camera.fov), renderEngine.get_aspect_ratio(), camera.nearPlane, camera.farPlane);
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
		shader.second.update_global_uniform_constant("camera", renderEngine.get_frame(), &obj, sizeof(CameraBufferObject), 0);
	}
}

ID minty::RenderSystem::create_texture(TextureBuilder const& builder)
{
	return _textures.emplace(builder.name, Texture(builder, get_engine(), get_scene_id()));
}

ID minty::RenderSystem::create_sprite(SpriteBuilder const& builder)
{
	return _sprites.emplace(builder.name, Sprite(builder, get_engine(), get_scene_id()));
}

ID minty::RenderSystem::create_shader(ShaderBuilder const& builder)
{
	return _shaders.emplace(builder.name, Shader(builder, get_engine(), get_scene_id()));
}

ID minty::RenderSystem::create_shader_pass(ShaderPassBuilder const& builder)
{
	return _shaderPasses.emplace(builder.name, ShaderPass(builder, get_engine(), get_scene_id()));
}

ID minty::RenderSystem::create_material_template(MaterialTemplateBuilder const& builder)
{
	return _materialTemplates.emplace(builder.name, MaterialTemplate(builder, get_engine(), get_scene_id()));
}

ID minty::RenderSystem::create_material(MaterialBuilder const& builder)
{
	return _materials.emplace(builder.name, Material(builder, get_engine(), get_scene_id()));
}

ID minty::RenderSystem::create_mesh()
{
	// just create a brand new mesh
	return _meshes.emplace(Mesh(get_engine(), get_scene_id()));
}

ID minty::RenderSystem::get_or_create_mesh(String const& name)
{
	// if contains name, return it
	if (_meshes.contains(name))
	{
		return _meshes.get_id(name);
	}

	// create new
	return _meshes.emplace(name, Mesh(get_engine(), get_scene_id()));
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
	ID id = _meshes.emplace(name, Mesh(get_engine(), get_scene_id()));
	Mesh& mesh = _meshes.at(id);

	switch (type)
	{
	case MeshType::Custom:
		// do nothing for now
		Console::todo("MeshType::Custom");
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

ID minty::RenderSystem::find_texture(String const& name, ID const defaultId) const
{
	if (!_textures.contains(name))
	{
		return defaultId;
	}

	return _textures.get_id(name);
}

ID minty::RenderSystem::find_sprite(String const& name, ID const defaultId) const
{
	if (!_sprites.contains(name))
	{
		return defaultId;
	}

	return _sprites.get_id(name);
}

ID minty::RenderSystem::find_shader(String const& name, ID const defaultId) const
{
	if (!_shaders.contains(name))
	{
		return defaultId;
	}

	return _shaders.get_id(name);
}

ID minty::RenderSystem::find_shader_pass(String const& name, ID const defaultId) const
{
	if (!_shaderPasses.contains(name))
	{
		return defaultId;
	}

	return _shaderPasses.get_id(name);
}

ID minty::RenderSystem::find_material_template(String const& name, ID const defaultId) const
{
	if (!_materialTemplates.contains(name))
	{
		return defaultId;
	}

	return _materialTemplates.get_id(name);
}

ID minty::RenderSystem::find_material(String const& name, ID const defaultId) const
{
	if (!_materials.contains(name))
	{
		return defaultId;
	}

	return _materials.get_id(name);
}

ID minty::RenderSystem::find_mesh(String const& name, ID const defaultId) const
{
	if (!_meshes.contains(name))
	{
		return defaultId;
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
	if (Asset::check(path, nullptr, false))
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
	if (Asset::check(path, SPRITE_EXTENSION, false))
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
		.coordinateMode = from_string_coordinate_mode(reader.read_string("coordinateMode")),
		.minCoords = reader.read_object("min", Vector2(0.0f, 0.0f)),
		.maxCoords = reader.read_object("max", Vector2(1.0f, 1.0f)),
		.pivot = reader.read_object("pivot", Vector2(0.5f, 0.5f)),
	};

	return create_sprite(builder);
}

ID minty::RenderSystem::load_shader(Path const& path)
{
	return load_shader(path, path.stem().string());
}

ID minty::RenderSystem::load_shader(Path const& path, String const& name)
{
	if (Asset::check(path, SHADER_EXTENSION, false))
	{
		return ERROR_ID;
	}

	Node meta = Asset::load_node(path);

	ShaderBuilder builder;
	builder.name = name;

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

	return create_shader(builder);
}

ID minty::RenderSystem::load_shader_pass(Path const& path)
{
	return load_shader_pass(path, path.stem().string());
}

ID minty::RenderSystem::load_shader_pass(Path const& path, String const& name)
{
	if (Asset::check(path, SHADER_PASS_EXTENSION, false))
	{
		return ERROR_ID;
	}

	Node meta = Asset::load_node(path);
	Reader reader(meta);

	ShaderPassBuilder builder;
	builder.name = name;
	builder.shaderId = find_shader(reader.read_string("shader", meta.to_string()));
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

	return create_shader_pass(builder);
}

ID minty::RenderSystem::load_material_template(Path const& path)
{
	return load_material_template(path, path.stem().string());
}

ID minty::RenderSystem::load_material_template(Path const& path, String const& name)
{
	if (Asset::check(path, MATERIAL_TEMPLATE_EXTENSION, false))
	{
		return ERROR_ID;
	}

	Node meta = Asset::load_node(path);

	MaterialTemplateBuilder builder;
	builder.name = name;

	std::vector<Node const*> nodes = meta.find_all("pass");
	for (auto const* child : nodes)
	{
		builder.shaderPassIds.push_back(find_shader_pass(child->to_string()));
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
	if (Asset::check(path, MATERIAL_EXTENSION, false))
	{
		return ERROR_ID;
	}

	Node meta = Asset::load_node(path);
	Reader reader(meta);

	MaterialBuilder builder;
	builder.name = name;

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
	if (Asset::check(path, nullptr, false))
	{
		return ERROR_ID;
	}

	String extension = path.extension().string();

	if (extension != ".obj")
	{
		Console::error(std::format("Cannot load_animation mesh from file type \"{}\".", extension));
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
	MINTY_ASSERT(_textures.contains(id), std::format("RenderSystem::get_texture(): Textures does not contain the ID {}.", id));
	
	return _textures.at(id);
}

Texture const& minty::RenderSystem::get_texture(ID const id) const
{
	MINTY_ASSERT(_textures.contains(id), std::format("RenderSystem::get_texture(): Textures does not contain the ID {}.", id));

	return _textures.at(id);
}

Sprite& minty::RenderSystem::get_sprite(ID const id)
{
	MINTY_ASSERT(_sprites.contains(id), std::format("RenderSystem::get_sprite(): Sprites does not contain the ID {}.", id));

	return _sprites.at(id);
}

Sprite const& minty::RenderSystem::get_sprite(ID const id) const
{
	MINTY_ASSERT(_sprites.contains(id), std::format("RenderSystem::get_sprite(): Sprites does not contain the ID {}.", id));

	return _sprites.at(id);
}

Shader& minty::RenderSystem::get_shader(ID const id)
{
	MINTY_ASSERT(_shaders.contains(id), std::format("RenderSystem::get_shader(): Shaders does not contain the ID {}.", id));

	return _shaders.at(id);
}

Shader const& minty::RenderSystem::get_shader(ID const id) const
{
	MINTY_ASSERT(_shaders.contains(id), std::format("RenderSystem::get_shader(): Shaders does not contain the ID {}.", id));

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
	MINTY_ASSERT(_shaderPasses.contains(id), std::format("RenderSystem::get_shader_pass(): ShaderPasses does not contain the ID {}.", id));

	return _shaderPasses.at(id);
}

ShaderPass const& minty::RenderSystem::get_shader_pass(ID const id) const
{
	MINTY_ASSERT(_shaderPasses.contains(id), std::format("RenderSystem::get_shader_pass(): ShaderPasses does not contain the ID {}.", id));

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
	MINTY_ASSERT(_materialTemplates.contains(id), std::format("RenderSystem::get_material_template(): MaterialTemplates does not contain the ID {}.", id));

	return _materialTemplates.at(id);
}

MaterialTemplate const& minty::RenderSystem::get_material_template(ID const id) const
{
	MINTY_ASSERT(_materialTemplates.contains(id), std::format("RenderSystem::get_material_template(): MaterialTemplates does not contain the ID {}.", id));

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
	MINTY_ASSERT(_materials.contains(id), std::format("RenderSystem::get_material(): Materials does not contain the ID {}.", id));

	return _materials.at(id);
}

Material const& minty::RenderSystem::get_material(ID const id) const
{
	MINTY_ASSERT(_materials.contains(id), std::format("RenderSystem::get_material(): Materials does not contain the ID {}.", id));

	return _materials.at(id);
}

Mesh& minty::RenderSystem::get_mesh(ID const id)
{
	MINTY_ASSERT(_meshes.contains(id), std::format("RenderSystem::get_mesh(): Meshes does not contain the ID {}.", id));

	return _meshes.at(id);
}

Mesh const& minty::RenderSystem::get_mesh(ID const id) const
{
	MINTY_ASSERT(_meshes.contains(id), std::format("RenderSystem::get_mesh(): Meshes does not contain the ID {}.", id));

	return _meshes.at(id);
}


void minty::RenderSystem::set_main_camera(Entity const entity)
{
	_mainCamera = entity;
}

void minty::RenderSystem::load_descriptor_values(std::unordered_map<String, Dynamic>& values, Node const& node, std::vector<UniformConstantInfo> const& infos) const
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
			ID* ids = new ID[size];
			for (size_t i = 0; i < size; i++)
			{
				// if child with the index name exists, use that
				if (Node const* c = child->find(std::to_string(i)))
				{
					// should be a texture name, so load that into ids
					ids[i] = find_texture(c->to_string());
				}
				else
				{
					// if index name does not exist, show warning and set to ERROR_ID
					Console::warn(std::format("Failed to load_animation texture with index {} into descriptor named \"{}\".", i, info.name));
					ids[i] = ERROR_ID;
				}
			}

			// add to values
			values.emplace(child->get_name(), Dynamic(ids, sizeof(ID) * size));
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

void minty::RenderSystem::serialize(Writer& writer) const
{
	Console::todo("RenderSystem::serialize()");
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
		for(Node const& atlasNode : atlasesNode->get_children())
		{
			Reader atlasReader(atlasNode, reader.get_data());

			TextureAtlasBuilder builder
			{
				.textureId = find_texture(atlasReader.read_string("texture")),
				.materialId = find_material(atlasReader.read_string("material")),
				.coordinateMode = from_string_coordinate_mode(atlasReader.read_string("coordinateMode")),
				.slice = atlasReader.read_object("slice", Vector2(0.0f, 0.0f)),
				.pivot = atlasReader.read_object("pivot", Vector2(0.5f, 0.5f))
			};

			TextureAtlas atlas(builder, get_engine(), get_scene_id());

			if (Node const* spritesNode = atlasNode.find("sprites"))
			{
				// determine what to do with the sprites
				if (spritesNode->find("all"))
				{
					// create all sprites
					atlas.create_all();
				}
				else
				{
					// create sprites at specific X Y coordinates
					std::vector<Node const*> nodes = spritesNode->find_all("sprite");
					for (Node const* n : nodes)
					{
						Reader nReader(*n, reader.get_data());
						int x = nReader.read_int("x");
						int y = nReader.read_int("y");
						Vector2 pivot;
						nReader.read_object_ref("pivot", pivot, Vector2(0.5f, 0.5f));
						CoordinateMode coordinateMode = from_string_coordinate_mode(nReader.read_string("coordinateMode"));

						atlas.create_sprite(x, y, pivot, coordinateMode);
					}

					// create slices at specific offsets
					nodes = spritesNode->find_all("slice");
					for (Node const* n : nodes)
					{
						Reader nReader(*n, reader.get_data());
						Vector2 minCoords = nReader.read_object("min", Vector2(0.0f, 0.0f));
						Vector2 maxCoords = nReader.read_object("max", Vector2(1.0f, 1.0f));
						CoordinateMode coordinateMode = from_string_coordinate_mode(nReader.read_string("coordinateMode"));

						atlas.slice_sprite(minCoords, maxCoords, coordinateMode);
					}
				}
			}
		}		
	}
}
