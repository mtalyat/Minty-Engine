#include "pch.h"
#include "M_RenderSystem.h"

#include "M_RenderEngine.h"
#include "M_Asset.h"
#include "M_Runtime.h"
#include "M_AssetEngine.h"
#include "M_Builtin.h"
#include "M_Vector.h"
#include "M_GLM.hpp"

#include "M_EntityRegistry.h"
#include "M_CameraComponent.h"
#include "M_TransformComponent.h"
#include "M_EnabledComponent.h"
#include "M_DrawCallObjectInfo.h"

#include "M_Reader.h"
#include "M_Writer.h"

using namespace minty;
using namespace minty;
using namespace minty::vk;
using namespace minty::Builtin;

minty::RenderSystem::RenderSystem(Runtime& engine, Scene& scene)
	: System::System("Render", engine, scene)
{}

minty::RenderSystem::~RenderSystem()
{}

void minty::RenderSystem::reset()
{
	_shaders.clear();
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

	// update camera in renderer if it is enabled
	if (entityRegistry.try_get<EnabledComponent>(_mainCamera))
	{
		CameraComponent const& camera = entityRegistry.get<CameraComponent>(_mainCamera);
		TransformComponent const& transformComponent = entityRegistry.get<TransformComponent>(_mainCamera);

		update_camera(camera, transformComponent);
	}
}

void minty::RenderSystem::update_camera(CameraComponent const& camera, TransformComponent const& transform)
{
	Vector4 matPos = transform.globalMatrix[3];
	Vector3 globalPos = Vector3(matPos.x, matPos.y, matPos.z);

	Matrix4 view = glm::lookAt(globalPos, globalPos + forward(transform.localRotation), Vector3(0.0f, 1.0f, 0.0f));

	// TODO: don't use lookat
	// maybe invert global?

	RenderEngine& renderEngine = get_runtime().get_render_engine();

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
		shader->update_global_uniform_constant("camera", renderEngine.get_frame(), &obj, sizeof(CameraBufferObject), 0);
	}
}

void minty::RenderSystem::set_main_camera(Entity const entity)
{
	_mainCamera = entity;
}

void minty::RenderSystem::serialize(Writer& writer) const
{
	Console::todo("RenderSystem::serialize()");
}

void minty::RenderSystem::deserialize(Reader const& reader)
{
	register_assets(reader, "textures", [](AssetEngine& assets, Path const& path) { return assets.load_texture(path); });
	register_assets(reader, "shaders", [this](AssetEngine& assets, Path const& path)
		{
			// load asset, also register with this system for other uses
			Shader* shader = assets.load_shader(path);
			if (shader) _shaders.emplace(shader);
			return shader;
		});
	register_assets(reader, "shaderPasses", [](AssetEngine& assets, Path const& path) { return assets.load_shader_pass(path); });
	register_assets(reader, "materialTemplates", [](AssetEngine& assets, Path const& path) { return assets.load_material_template(path); });
	register_assets(reader, "materials", [](AssetEngine& assets, Path const& path) { return assets.load_material(path); });

	register_assets(reader, "meshes", [](AssetEngine& assets, Path const& path) { return assets.load_mesh(path); });
	register_assets(reader, "sprites", [](AssetEngine& assets, Path const& path) { return assets.load_sprite(path); });

	Console::todo("RenderSystem::deserialize(): TextureAtlases.");
	// load as atlas type in case user wants to use it dynamically in code

	//// custom parse atlases
	//Node const& node = reader.get_node();
	//if (Node const* atlasesNode = node.find("atlases"))
	//{
	//	for(Node const& atlasNode : atlasesNode->get_children())
	//	{
	//		Reader atlasReader(atlasNode, reader.get_data());

	//		TextureAtlasBuilder builder
	//		{
	//			.texture = assets.get<Texture>(atlasReader.read_uuid("texture")),
	//			.material = assets.get<Material>(atlasReader.read_uuid("material")),
	//			.coordinateMode = from_string_coordinate_mode(atlasReader.read_string("coordinateMode")),
	//			.slice = atlasReader.read_object("slice", Vector2(0.0f, 0.0f)),
	//			.pivot = atlasReader.read_object("pivot", Vector2(0.5f, 0.5f))
	//		};

	//		TextureAtlas atlas(builder, get_runtime());

	//		if (Node const* spritesNode = atlasNode.find("sprites"))
	//		{
	//			// determine what to do with the sprites
	//			if (spritesNode->find("all"))
	//			{
	//				// create all sprites
	//				atlas.create_all();
	//			}
	//			else
	//			{
	//				// create sprites at specific X Y coordinates
	//				std::vector<Node const*> nodes = spritesNode->find_all("sprite");
	//				for (Node const* n : nodes)
	//				{
	//					Reader nReader(*n, reader.get_data());
	//					int x = nReader.read_int("x");
	//					int y = nReader.read_int("y");
	//					Vector2 pivot;
	//					nReader.read_object_ref("pivot", pivot, Vector2(0.5f, 0.5f));
	//					CoordinateMode coordinateMode = from_string_coordinate_mode(nReader.read_string("coordinateMode"));

	//					atlas.create_sprite(x, y, pivot, coordinateMode);
	//				}

	//				// create slices at specific offsets
	//				nodes = spritesNode->find_all("slice");
	//				for (Node const* n : nodes)
	//				{
	//					Reader nReader(*n, reader.get_data());
	//					Vector2 minCoords = nReader.read_object("min", Vector2(0.0f, 0.0f));
	//					Vector2 maxCoords = nReader.read_object("max", Vector2(1.0f, 1.0f));
	//					CoordinateMode coordinateMode = from_string_coordinate_mode(nReader.read_string("coordinateMode"));

	//					atlas.slice_sprite(minCoords, maxCoords, coordinateMode);
	//				}
	//			}
	//		}
	//	}		
	//}
}
