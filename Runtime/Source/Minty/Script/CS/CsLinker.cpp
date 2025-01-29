#include "pch.h"
#include "CsLinker.h"

#include "Minty/Core/Application.h"
#include "Minty/Core/Linker.h"
#include "Minty/Core/Math.h"
#include "Minty/Window/Window.h"
#include "Minty/Component/AllComponents.h"
#include "Minty/Scene/Scene.h"
#include "Minty/Scene/SceneManager.h"
#include "Minty/Script/CS/CsScriptEngine.h"
#include "Minty/System/AllSystems.h"

using namespace Minty;

constexpr static char const* INTERNAL_CLASS_NAME = "Runtime";

#define ADD_INTERNAL_CALL(csharpName, cppName) mono_add_internal_call(std::format("{}.{}::{}", MINTY_NAME_ENGINE, INTERNAL_CLASS_NAME, csharpName).c_str(), cppName)

#pragma region Util

static SceneManager& util_get_scene_manager()
{
	return Application::instance().get_scene_manager();
}

static Scene& util_get_scene()
{
	Ref<Scene> scene = util_get_scene_manager().get_working_scene();
	MINTY_ASSERT(scene != nullptr);
	return *scene;
}

static EntityRegistry& util_get_entity_registry()
{
	return util_get_scene().get_entity_registry();
}

static Window& util_get_window(UUID_t const id)
{
	Ref<Window> window = WindowManager::get_window(id);
	MINTY_ASSERT(window != nullptr);
	return *window.get();
}

static Window& util_get_main_window()
{
	Ref<Window> window = WindowManager::get_main();
	MINTY_ASSERT(window != nullptr);
	return *window.get();
}

static Entity util_get_entity(UUID_t id)
{
	EntityRegistry& registry = util_get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	return entity;
}

static CameraComponent& util_get_camera_component(UUID_t const id)
{
	MINTY_ASSERT(id);

	EntityRegistry& registry = util_get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	CameraComponent* component = registry.try_get<CameraComponent>(entity);
	MINTY_ASSERT(component != nullptr);

	return *component;
}

static MeshComponent& util_get_mesh_component(UUID_t const id)
{
	MINTY_ASSERT(id);

	EntityRegistry& registry = util_get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	MeshComponent* component = registry.try_get<MeshComponent>(entity);
	MINTY_ASSERT(component != nullptr);

	return *component;
}

static TransformComponent& util_get_transform_component(UUID_t const id, bool setDirty)
{
	MINTY_ASSERT(id);

	EntityRegistry& registry = util_get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// if set dirty, do that
	if (setDirty)
	{
		registry.dirty(entity);
	}

	TransformComponent* component = registry.try_get<TransformComponent>(entity);
	MINTY_ASSERT(component != nullptr);

	return *component;
}

static MonoObject* util_get_mono_object(UUID const id)
{
	Ref<ScriptObject> scriptObject = ScriptEngine::get_object(id);

	if (scriptObject == nullptr)
	{
		return nullptr;
	}

	return static_cast<MonoObject*>(scriptObject->get_native());
}

static MonoObject* util_get_mono_asset(UUID const id, String const& classFullName)
{
	Ref<ScriptClass> scriptClass = ScriptEngine::find_class(classFullName);

	MINTY_ASSERT(scriptClass != nullptr);

	Ref<ScriptObject> scriptObject = ScriptEngine::get_or_create_object_asset(id, scriptClass);

	if (scriptObject == nullptr)
	{
		return nullptr;
	}

	return static_cast<MonoObject*>(scriptObject->get_native());
}

#pragma endregion

#pragma region Components

#pragma region Camera

static Int camera_get_perspective(UUID_t id)
{
	CameraComponent& camera = util_get_camera_component(id);

	return static_cast<Int>(camera.camera.get_perspective());
}

static void camera_set_perspective(UUID_t id, Int value)
{
	CameraComponent& camera = util_get_camera_component(id);

	camera.camera.set_perspective(static_cast<Perspective>(value));
}

static float camera_get_fov(UUID_t id)
{
	CameraComponent& camera = util_get_camera_component(id);

	return camera.camera.get_fov();
}

static void camera_set_fov(UUID_t id, float value)
{
	CameraComponent& camera = util_get_camera_component(id);

	camera.camera.set_fov(value);
}

static float camera_get_near(UUID_t id)
{
	CameraComponent& camera = util_get_camera_component(id);

	return camera.camera.get_near();
}

static void camera_set_near(UUID_t id, float value)
{
	CameraComponent& camera = util_get_camera_component(id);

	camera.camera.set_near(value);
}

static float camera_get_far(UUID_t id)
{
	CameraComponent& camera = util_get_camera_component(id);

	return camera.camera.get_far();
}

static void camera_set_far(UUID_t id, float value)
{
	CameraComponent& camera = util_get_camera_component(id);

	camera.camera.set_far(value);
}

static MonoObject* camera_get_main()
{
	Scene& scene = util_get_scene();
	EntityRegistry& registry = scene.get_entity_registry();

	// set main in render system
	RenderSystem* renderSystem = scene.get_system_registry().find<RenderSystem>();
	MINTY_ASSERT(renderSystem != nullptr);

	Entity entity = renderSystem->get_main_camera();

	if (entity == NULL_ENTITY)
	{
		return nullptr;
	}

	UUID id = registry.get_id(entity);

	if (!registry.all_of<CameraComponent>(entity))
	{
		return nullptr;
	}

	CameraComponent& camera = registry.get<CameraComponent>(entity);

	Ref<ScriptClass> scriptClass = ScriptEngine::find_class("MintyEngine.Camera");

	MINTY_ASSERT(scriptClass);

	UUID entityId = registry.get_id(entity);
	Ref<ScriptObject> componentObject = ScriptEngine::get_or_create_object_component(camera.get_id(), entityId, scriptClass);
	return static_cast<MonoObject*>(componentObject->get_native());
}

static void camera_set_main(UUID_t id)
{
	MINTY_ASSERT(id);

	Scene& scene = util_get_scene();
	EntityRegistry& registry = scene.get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);
	MINTY_ASSERT(registry.all_of<CameraComponent>(entity));

	// set main in render system
	RenderSystem* renderSystem = scene.get_system_registry().find<RenderSystem>();
	MINTY_ASSERT(renderSystem != nullptr);

	renderSystem->set_main_camera(entity);
}

static Int camera_get_color(UUID_t id)
{
	CameraComponent& camera = util_get_camera_component(id);

	return static_cast<Int>(camera.camera.get_color());
}

static void camera_set_color(UUID_t id, Int value)
{
	CameraComponent& camera = util_get_camera_component(id);

	camera.camera.set_color(Color(value));
}

static void camera_set_render_target(UUID_t id, UUID_t renderTargetID)
{
	CameraComponent& camera = util_get_camera_component(id);

	Ref<RenderTarget> renderTarget = AssetManager::get<RenderTarget>(renderTargetID);

	camera.camera.set_render_target(renderTarget);
}

static UUID_t camera_get_render_target(UUID_t id)
{
	CameraComponent& camera = util_get_camera_component(id);

	Ref<RenderTarget> renderTarget = camera.camera.get_render_target();

	return renderTarget->id();
}

#pragma endregion

#pragma region MeshRenderer

static int mesh_renderer_get_type(UUID_t id)
{
	MeshComponent& component = util_get_mesh_component(id);

	return static_cast<int>(component.type);
}

static void mesh_renderer_set_type(UUID_t id, int type)
{
	MeshComponent& component = util_get_mesh_component(id);

	component.type = static_cast<MeshType>(type);
}

static MonoObject* mesh_renderer_get_mesh(UUID_t id)
{
	MeshComponent& component = util_get_mesh_component(id);

	Ref<Mesh> mesh = component.mesh;

	if (mesh == nullptr)
	{
		return nullptr;
	}

	return util_get_mono_asset(mesh->id(), "Mesh");
}

static void mesh_renderer_set_mesh(UUID_t id, UUID_t meshId)
{
	MeshComponent& component = util_get_mesh_component(id);

	component.mesh = AssetManager::get<Mesh>(meshId);
}

static MonoObject* mesh_renderer_get_material(UUID_t id)
{
	MeshComponent& component = util_get_mesh_component(id);

	Ref<Material> material = component.material;

	if (material == nullptr)
	{
		return nullptr;
	}

	return util_get_mono_asset(material->id(), "Material");
}

static void mesh_renderer_set_material(UUID_t id, UUID_t materialId)
{
	MeshComponent& component = util_get_mesh_component(id);

	component.material = AssetManager::get<Material>(materialId);
}

#pragma endregion

#pragma region Transform

static void transform_get_local_position(UUID_t id, Float3* position)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*position = component.localPosition;
}

static void transform_set_local_position(UUID_t id, Float3* position)
{
	TransformComponent& component = util_get_transform_component(id, true);

	component.localPosition = *position;
}

static void transform_get_local_rotation(UUID_t id, Float4* rotation)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*rotation = Float4(component.localRotation.x, component.localRotation.y, component.localRotation.z, component.localRotation.w);
}

static void transform_set_local_rotation(UUID_t id, Float4* rotation)
{
	TransformComponent& component = util_get_transform_component(id, true);

	component.localRotation = Quaternion(rotation->w, rotation->x, rotation->y, rotation->z);
}

static void transform_get_local_scale(UUID_t id, Float3* scale)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*scale = component.localScale;
}

static void transform_set_local_scale(UUID_t id, Float3* scale)
{
	TransformComponent& component = util_get_transform_component(id, true);

	component.localScale = *scale;
}

static void transform_get_global_position(UUID_t id, Float3* position)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*position = component.get_global_position();
}

//static void transform_set_global_position(UUID_t id, Float3* position)
//{
//	TransformComponent& component = util_get_transform_component(id, true);
//
//	component.localPosition = *position;
//}

static void transform_get_global_rotation(UUID_t id, Float4* rotation)
{
	TransformComponent& component = util_get_transform_component(id, false);

	Quaternion rot = component.get_global_rotation();

	*rotation = Float4(rot.x, rot.y, rot.z, rot.w);
}

//static void transform_set_global_rotation(UUID_t id, Float4* rotation)
//{
//	TransformComponent& component = util_get_transform_component(id, true);
//
//	component.localRotation = Quaternion(rotation->w, rotation->x, rotation->y, rotation->z);
//}

static void transform_get_global_scale(UUID_t id, Float3* scale)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*scale = component.get_global_scale();
}

//static void transform_set_global_scale(UUID_t id, Float3* scale)
//{
//	TransformComponent& component = util_get_transform_component(id, true);
//
//	component.localScale = *scale;
//}

static void transform_get_right(UUID_t id, Float3* direction)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*direction = component.get_right();
}

static void transform_get_up(UUID_t id, Float3* direction)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*direction = component.get_up();
}

static void transform_get_forward(UUID_t id, Float3* direction)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*direction = component.get_forward();
}

#pragma endregion

#pragma endregion

#pragma region Core

#pragma region Console

static void console_log(MonoString* string)
{
	Console::log(CsScriptEngine::from_mono_string(string));
}

static void console_log_color(MonoString* string, Int color)
{
	Console::log_color(CsScriptEngine::from_mono_string(string), static_cast<Console::Color>(color));
}

static void console_warn(MonoString* string)
{
	Console::warn(CsScriptEngine::from_mono_string(string));
}

static void console_error(MonoString* string)
{
	Console::error(CsScriptEngine::from_mono_string(string));
}

#pragma endregion

#pragma region Time

static float time_get_total()
{
	return Application::instance().get_time().total;
}

static float time_get_elapsed()
{
	return Application::instance().get_time().elapsed;
}

#pragma endregion

#pragma endregion

#pragma region Input

#pragma region Cursor

static CursorMode cursor_get_mode()
{
	return util_get_main_window().get_cursor_mode();
}

static void cursor_set_mode(CursorMode mode)
{
	util_get_main_window().set_cursor_mode(mode);
}

#pragma endregion

#pragma endregion

#pragma region Object

#pragma region Object

static void object_destroy_entity(UUID_t id)
{
	EntityRegistry& registry = util_get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	registry.destroy(entity);
}

static void object_destroy_immediate_entity(UUID_t id)
{
	EntityRegistry& registry = util_get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	registry.destroy_immediate(entity);
}

#pragma endregion

#pragma region Entity

static MonoObject* entity_find(UUID_t id)
{
	// ID of 0 is null always
	if (!id)
	{
		return nullptr;
	}

	UUID uuid = id;

	// get the entity
	EntityRegistry& registry = util_get_entity_registry();
	Entity entity = registry.find_by_id(uuid);

	// get its object
	Ref<ScriptObject> scriptObject = ScriptEngine::get_or_create_object_entity(uuid);

	return static_cast<MonoObject*>(scriptObject->get_native());
}

static MonoString* entity_get_name(UUID_t id)
{
	if (!id) return CsScriptEngine::to_mono_string("");

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the name
	String name = registry.get_name(entity);

	// convert to mono string and return
	return CsScriptEngine::to_mono_string(name);
}

static void entity_set_name(UUID_t id, MonoString* string)
{
	if (!id) return;

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// set the name
	String name = CsScriptEngine::from_mono_string(string);
	registry.set_name(entity, name);
}

static MonoString* entity_get_tag(UUID_t id)
{
	if (!id) return CsScriptEngine::to_mono_string("");

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the name
	String name = registry.get_tag(entity);

	// convert to mono string and return
	return CsScriptEngine::to_mono_string(name);
}

static void entity_set_tag(UUID_t id, MonoString* string)
{
	if (!id) return;

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// set the name
	String name = CsScriptEngine::from_mono_string(string);
	registry.set_tag(entity, name);
}

static Layer entity_get_layer(UUID_t id)
{
	if (!id) return LAYER_NONE;

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the layer
	return registry.get_layer(entity);
}

static void entity_set_layer(UUID_t id, Layer layer)
{
	if (!id) return;

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// set the layer
	registry.set_layer(entity, layer);
}

static void entity_set_enabled(UUID_t id, bool enabled)
{
	MINTY_ASSERT(id);

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	if (enabled)
	{
		registry.enable(entity);
	}
	else
	{
		registry.disable(entity);
	}
}

static bool entity_get_enabled(UUID_t id)
{
	MINTY_ASSERT(id);

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	return registry.all_of<EnabledComponent>(entity);
}

static MonoObject* entity_add_component(UUID_t id, MonoReflectionType* reflectionType)
{
	MINTY_ASSERT(id);

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the component name
	MonoType* monoType = mono_reflection_type_get_type(reflectionType);
	auto found = Linker::s_data.types.find(mono_type_get_name(monoType));
	if (found == Linker::s_data.types.end())
	{
		MINTY_ABORT(std::format("{} cannot be added, it has not been registered.", mono_type_get_name(monoType)));
		return nullptr;
	}
	Ref<ScriptClass> scriptClass = found->second;

	// get the component by name
	ScriptObjectComponent* component = static_cast<ScriptObjectComponent*>(registry.get_by_name(scriptClass->get_full_name(), entity));

	// if null, add the component
	if (!component)
	{
		component = static_cast<ScriptObjectComponent*>(registry.emplace_by_name(scriptClass->get_full_name(), entity));
	}

	// create a new object for this component, if needed
	UUID entityId = registry.get_id(entity);
	Ref<ScriptObject> componentObject = ScriptEngine::get_or_create_object_component(component->get_id(), entityId, scriptClass);
	return static_cast<MonoObject*>(componentObject->get_native());
}

static MonoObject* entity_get_component(UUID_t id, MonoReflectionType* reflectionType)
{
	MINTY_ASSERT(id);

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the component name
	MonoType* monoType = mono_reflection_type_get_type(reflectionType);
	auto found = Linker::s_data.types.find(mono_type_get_name(monoType));
	if (found == Linker::s_data.types.end())
	{
		MINTY_ABORT(std::format("{} cannot be gotten, it has not been registered.", mono_type_get_name(monoType)));
		// type not found
		return nullptr;
	}
	Ref<ScriptClass> scriptClass = found->second;

	// if MintyEngine, remove from name
	String scriptClassName = scriptClass->get_full_name();
	if (scriptClassName.starts_with("MintyEngine."))
	{
		scriptClassName = scriptClassName.substr(12, scriptClassName.length() - 12);
	}

	// get the component by name
	ScriptObjectComponent* component = static_cast<ScriptObjectComponent*>(registry.get_by_name(scriptClassName, entity));

	// if null, return null
	if (!component)
	{
		return nullptr;
	}

	// create a new object for this component
	UUID entityId = registry.get_id(entity);
	Ref<ScriptObject> componentObject = ScriptEngine::get_or_create_object_component(component->get_id(), entityId, scriptClass);
	return static_cast<MonoObject*>(componentObject->get_native());
}

static void entity_remove_component(UUID_t id, MonoReflectionType* reflectionType)
{
	MINTY_ASSERT(id);

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the component name
	MonoType* monoType = mono_reflection_type_get_type(reflectionType);
	auto found = Linker::s_data.types.find(mono_type_get_name(monoType));
	if (found == Linker::s_data.types.end())
	{
		MINTY_ABORT(std::format("{} cannot be removed, it has not been registered.", mono_type_get_name(monoType)));
		// type not found
		return;
	}
	Ref<ScriptClass> scriptClass = found->second;

	// destroy the component by name
	registry.destroy(entity, scriptClass->get_full_name());
}

static MonoObject* entity_get_parent(UUID_t id)
{
	MINTY_ASSERT(id);

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the parent
	Entity parent = registry.get_parent(entity);

	// get the ID
	UUID parentId = registry.get_id(parent);

	if (!parentId.valid()) return nullptr;

	return static_cast<MonoObject*>(ScriptEngine::get_or_create_object_entity(parentId)->get_native());
}

static void entity_set_parent(UUID_t id, UUID_t parentId)
{
	MINTY_ASSERT(id);

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the parent
	Entity parentEntity = registry.find_by_id(parentId);

	// set the parent
	registry.set_parent(entity, parentEntity);
}

static Int entity_get_child_count(UUID_t id)
{
	MINTY_ASSERT(id);

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	size_t childCount = registry.get_child_count(entity);

	MINTY_ASSERT_MESSAGE(childCount <= INT_MAX, "Entity child count exceeded INT_MAX.");

	return static_cast<Int>(childCount);
}

static MonoObject* entity_get_child(UUID_t id, Int index)
{
	MINTY_ASSERT(id);

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the child
	Entity child = registry.get_child(entity, index);

	UUID childId = registry.get_id(child);

	if (!childId.valid()) return nullptr;

	return static_cast<MonoObject*>(ScriptEngine::get_or_create_object_entity(childId)->get_native());
}

static MonoObject* entity_clone(UUID_t id)
{
	MINTY_ASSERT(id);

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// clone it
	Entity clone = registry.clone(entity);

	// get the ID
	UUID cloneId = registry.get_id(clone);

	if (!cloneId.valid()) return nullptr;

	return static_cast<MonoObject*>(ScriptEngine::get_or_create_object_entity(cloneId)->get_native());
}

#pragma endregion

#pragma endregion

#pragma region Render

#pragma region Image

static UUID_t image_create(Int format, Int type, Int tiling, Int aspect, Int usage, UInt width, UInt height, Bool immutable)
{
	ImageBuilder builder{};
	builder.id = UUID::create();
	builder.format = static_cast<Format>(format);
	builder.type = static_cast<ImageType>(type);
	builder.tiling = static_cast<ImageTiling>(tiling);
	builder.aspect = static_cast<ImageAspect>(aspect);
	builder.usage = static_cast<ImageUsage>(usage);
	builder.width = width;
	builder.height = height;
	builder.immutable = immutable;

	MINTY_LOG_FORMAT("Image(format={}({}), type={}({}), tiling={}({}), aspect={}({}), usage={}({}), width={}, height={}, immutable={})",
		format, to_string(builder.format), type, to_string(builder.type), tiling, to_string(builder.tiling), aspect, to_string(builder.aspect), usage, to_string(builder.usage), width, height, immutable);

	Owner<Image> image = Image::create(builder);

	AssetManager::emplace(image);

	MINTY_LOG_FORMAT("Created image with {} width and {} height, with ID: {}.", width, height, builder.id.data());

	return static_cast<unsigned long>(image->id().data());
}

#pragma endregion

#pragma region Material

static UUID_t material_create(MonoString* path)
{
	String pathString = CsScriptEngine::from_mono_string(path);
	Path pathPath = pathString;

	Ref<Material> material = AssetManager::load<Material>(pathPath);
	return material->id().data();
}

static MonoObject* material_get_template(UUID_t id)
{
	Ref<Material> material = AssetManager::get<Material>(id);

	Ref<MaterialTemplate> materialTemplate = material->get_template();

	if (materialTemplate == nullptr)
	{
		return nullptr;
	}

	return util_get_mono_asset(materialTemplate->id(), "MaterialTemplate");
}

static void material_set(UUID_t id, MonoString* name, int type, MonoString* value)
{
	Ref<Material> material = AssetManager::get<Material>(id);
	String nameString = CsScriptEngine::from_mono_string(name);
	Type typeType = static_cast<Type>(type);
	String valueString = CsScriptEngine::from_mono_string(value);

	switch (typeType)
	{
	case Type::Int:
	{
		Int temp = Parse::to_int(valueString);
		material->set_input(nameString, &temp);
		break;
	}
	case Type::Object:
	{
		UUID_t temp = static_cast<UUID_t>(Parse::to_size(valueString));
		Ref<Texture> texture = AssetManager::get<Texture>(temp);
		MINTY_ASSERT_FORMAT(texture != nullptr, "No Texture found with the ID {}.", to_string(UUID(temp)));
		void* rawTexture = texture.get();
		material->set_input(nameString, &rawTexture);
		break;
	}
	default:
		MINTY_ABORT_FORMAT("Unable to set Material input with value of type {}.", to_string(typeType));
		break;
	}
}

#pragma endregion

#pragma region Shader

static UUID_t shader_create(MonoString* path)
{
	String pathString = CsScriptEngine::from_mono_string(path);

	Ref<Shader> shader = AssetManager::load<Shader>(pathString);

	return shader->id();
}

#pragma endregion

#pragma endregion

#pragma region Scene

#pragma region SceneManager

static void scene_manager_load(MonoString* string)
{
	// unload old, destroy old, create new, load it
	SceneManager& sceneManager = util_get_scene_manager();

	// get current
	Ref<Scene> scene = sceneManager.get_loaded_scene();

	// if a scene is loaded, unload and destroy it
	if (scene != nullptr)
	{
		// unload current
		sceneManager.unload();
	}

	// get path
	String pathString = CsScriptEngine::from_mono_string(string);
	Path pathPath = Path(pathString);

	// load new scene
	sceneManager.load(pathPath);
}

#pragma endregion

#pragma endregion

#pragma region Window

#pragma region Window

static MonoObject* window_get(UUID_t id)
{
	return static_cast<MonoObject*>(ScriptEngine::get_object(id)->get_native());
}

static MonoObject* window_get_main()
{
	return static_cast<MonoObject*>(window_get(util_get_main_window().id()));
}

static MonoString* window_get_title(UUID_t id)
{
	return CsScriptEngine::to_mono_string(util_get_window(id).get_title());
}

static void window_set_title(UUID_t id, MonoString* string)
{
	util_get_window(id).set_title(CsScriptEngine::from_mono_string(string));
}

static void window_set_icon(UUID_t id, MonoString* string)
{
	util_get_window(id).set_icon(CsScriptEngine::from_mono_string(string));
}

static bool window_is_open(UUID_t id)
{
	return util_get_window(id).is_open();
}

static void window_close(UUID_t id)
{
	util_get_window(id).close();
}

static void window_maximize(UUID_t id)
{
	util_get_window(id).maximize();
}

static void window_minimize(UUID_t id)
{
	util_get_window(id).minimize();
}

static void window_restore(UUID_t id)
{
	util_get_window(id).restore();
}

#pragma endregion

#pragma endregion

void Minty::CsLinker::link()
{
	// link all the functions

#pragma region Components

#pragma region Camera
	ADD_INTERNAL_CALL("Camera_GetPerspective", camera_get_perspective);
	ADD_INTERNAL_CALL("Camera_SetPerspective", camera_set_perspective);
	ADD_INTERNAL_CALL("Camera_GetFov", camera_get_fov);
	ADD_INTERNAL_CALL("Camera_SetFov", camera_set_fov);
	ADD_INTERNAL_CALL("Camera_GetNear", camera_get_near);
	ADD_INTERNAL_CALL("Camera_SetNear", camera_set_near);
	ADD_INTERNAL_CALL("Camera_GetFar", camera_get_far);
	ADD_INTERNAL_CALL("Camera_SetFar", camera_set_far);
	ADD_INTERNAL_CALL("Camera_GetMain", camera_get_main);
	ADD_INTERNAL_CALL("Camera_SetMain", camera_set_main);
	ADD_INTERNAL_CALL("Camera_GetColor", camera_get_color);
	ADD_INTERNAL_CALL("Camera_SetColor", camera_set_color);
#pragma endregion

#pragma region MeshRenderer
	ADD_INTERNAL_CALL("MeshRenderer_GetType", mesh_renderer_get_type);
	ADD_INTERNAL_CALL("MeshRenderer_SetType", mesh_renderer_set_type);
	ADD_INTERNAL_CALL("MeshRenderer_GetMesh", mesh_renderer_get_mesh);
	ADD_INTERNAL_CALL("MeshRenderer_SetMesh", mesh_renderer_set_mesh);
	ADD_INTERNAL_CALL("MeshRenderer_GetMaterial", mesh_renderer_get_material);
	ADD_INTERNAL_CALL("MeshRenderer_SetMaterial", mesh_renderer_set_material);
#pragma endregion

#pragma region Transform
	ADD_INTERNAL_CALL("Transform_GetLocalPosition", transform_get_local_position);
	ADD_INTERNAL_CALL("Transform_SetLocalPosition", transform_set_local_position);
	ADD_INTERNAL_CALL("Transform_GetLocalRotation", transform_get_local_rotation);
	ADD_INTERNAL_CALL("Transform_SetLocalRotation", transform_set_local_rotation);
	ADD_INTERNAL_CALL("Transform_GetLocalScale", transform_get_local_scale);
	ADD_INTERNAL_CALL("Transform_SetLocalScale", transform_set_local_scale);
	ADD_INTERNAL_CALL("Transform_GetGlobalPosition", transform_get_global_position);
	//ADD_INTERNAL_CALL("Transform_SetGlobalPosition", transform_set_global_position);
	ADD_INTERNAL_CALL("Transform_GetGlobalRotation", transform_get_global_rotation);
	//ADD_INTERNAL_CALL("Transform_SetGlobalRotation", transform_set_global_rotation);
	ADD_INTERNAL_CALL("Transform_GetGlobalScale", transform_get_global_scale);
	//ADD_INTERNAL_CALL("Transform_SetGlobalScale", transform_set_global_scale);
	ADD_INTERNAL_CALL("Transform_GetRight", transform_get_right);
	ADD_INTERNAL_CALL("Transform_GetUp", transform_get_up);
	ADD_INTERNAL_CALL("Transform_GetForward", transform_get_forward);
#pragma endregion

#pragma endregion

#pragma region Core

#pragma region Time
	ADD_INTERNAL_CALL("Time_GetTotalTime", time_get_total);
	ADD_INTERNAL_CALL("Time_GetElapsedTime", time_get_elapsed);
#pragma endregion

#pragma region Console
	ADD_INTERNAL_CALL("Console_Log", console_log);
	ADD_INTERNAL_CALL("Console_LogColor", console_log_color);
	ADD_INTERNAL_CALL("Console_Warn", console_warn);
	ADD_INTERNAL_CALL("Console_Error", console_error);
#pragma endregion

#pragma endregion

#pragma region Input

#pragma region Cursor
	ADD_INTERNAL_CALL("Cursor_GetMode", cursor_get_mode);
	ADD_INTERNAL_CALL("Cursor_SetMode", cursor_set_mode);
#pragma endregion

#pragma endregion

#pragma region Object

#pragma region Entity
	ADD_INTERNAL_CALL("Entity_Find", entity_find);
	ADD_INTERNAL_CALL("Entity_GetName", entity_get_name);
	ADD_INTERNAL_CALL("Entity_SetName", entity_set_name);
	ADD_INTERNAL_CALL("Entity_GetTag", entity_get_tag);
	ADD_INTERNAL_CALL("Entity_SetTag", entity_set_tag);
	ADD_INTERNAL_CALL("Entity_GetLayer", entity_get_layer);
	ADD_INTERNAL_CALL("Entity_SetLayer", entity_set_layer);
	ADD_INTERNAL_CALL("Entity_GetEnabled", entity_get_enabled);
	ADD_INTERNAL_CALL("Entity_SetEnabled", entity_set_enabled);
	ADD_INTERNAL_CALL("Entity_AddComponent", entity_add_component);
	ADD_INTERNAL_CALL("Entity_GetComponent", entity_get_component);
	ADD_INTERNAL_CALL("Entity_RemoveComponent", entity_remove_component);
	ADD_INTERNAL_CALL("Entity_GetParent", entity_get_parent);
	ADD_INTERNAL_CALL("Entity_SetParent", entity_set_parent);
	ADD_INTERNAL_CALL("Entity_GetChildCount", entity_get_child_count);
	ADD_INTERNAL_CALL("Entity_GetChild", entity_get_child);
	ADD_INTERNAL_CALL("Entity_Clone", entity_clone);
#pragma endregion

#pragma region Object
	ADD_INTERNAL_CALL("Object_DestroyEntity", object_destroy_entity);
	ADD_INTERNAL_CALL("Object_DestroyImmediateEntity", object_destroy_immediate_entity);
#pragma endregion

#pragma endregion

#pragma region Render

#pragma region Image
	ADD_INTERNAL_CALL("Image_Create", image_create);
#pragma endregion

#pragma region Material
	ADD_INTERNAL_CALL("Material_Create", material_create);
	ADD_INTERNAL_CALL("Material_GetTemplate", material_get_template);
	ADD_INTERNAL_CALL("Material_Set", material_set);
#pragma endregion

#pragma endregion

#pragma region Scene

#pragma region SceneManager
	ADD_INTERNAL_CALL("SceneManager_Load", scene_manager_load);
#pragma endregion

#pragma endregion

#pragma region Window

#pragma region Window
	ADD_INTERNAL_CALL("Window_GetMain", window_get_main);
	ADD_INTERNAL_CALL("Window_GetTitle", window_get_title);
	ADD_INTERNAL_CALL("Window_SetTitle", window_set_title);
	ADD_INTERNAL_CALL("Window_SetIcon", window_set_icon);
	ADD_INTERNAL_CALL("Window_IsOpen", window_is_open);
	ADD_INTERNAL_CALL("Window_Close", window_close);
	ADD_INTERNAL_CALL("Window_Maximize", window_maximize);
	ADD_INTERNAL_CALL("Window_Minimize", window_minimize);
	ADD_INTERNAL_CALL("Window_Restore", window_restore);
#pragma endregion

#pragma endregion

#undef ADD_INTERNAL_CALL
}
