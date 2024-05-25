#include "pch.h"
#include "M_ScriptLink.h"

#include "Minty/Libraries/M_Mono.h"

#include "Minty/Core/M_Window.h"
#include "Minty/Core/M_Application.h"
#include "Minty/Layers/M_DefaultLayer.h"

#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Audio/M_AudioEngine.h"
#include "Minty/Rendering/M_RenderEngine.h"
#include "Minty/Scripting/M_ScriptEngine.h"

#include "Minty/Scenes/M_Scene.h"
#include "Minty/Scenes/M_SceneManager.h"
#include "Minty/Entities/M_EntityRegistry.h"
#include "Minty/Systems/M_SystemRegistry.h"

#include "Minty/Files/M_File.h"
#include "Minty/Tools/M_Console.h"
#include "Minty/Scripting/M_ScriptObject.h"
#include "Minty/Scripting/M_ScriptArguments.h"

#include "Minty/Types/M_Vector.h"
#include "Minty/Types/M_Quaternion.h"
#include "Minty/Types/M_Matrix.h"

#include "Minty/Animation/M_AnimationSystem.h"
#include "Minty/Audio/M_AudioSystem.h"
#include "Minty/Rendering/M_RenderSystem.h"
#include "Minty/Scripting/M_ScriptSystem.h"
#include "Minty/UI/M_UISystem.h"

#include "Minty/Animation/M_AnimatorComponent.h"
#include "Minty/Audio/M_AudioListenerComponent.h"
#include "Minty/Audio/M_AudioSourceComponent.h"
#include "Minty/Rendering/M_CameraComponent.h"
#include "Minty/UI/M_CanvasComponent.h"
#include "Minty/Components/M_EnabledComponent.h"
#include "Minty/Rendering/M_MeshComponent.h"
#include "Minty/Components/M_NameComponent.h"
#include "Minty/Components/M_RelationshipComponent.h"
#include "Minty/Rendering/M_RenderableComponent.h"
#include "Minty/Scripting/M_ScriptComponent.h"
#include "Minty/Rendering/M_SpriteComponent.h"
#include "Minty/Components/M_TransformComponent.h"
#include "Minty/UI/M_UITransformComponent.h"

#include "Minty/Rendering/M_RenderSystem.h"

using namespace Minty;

struct ScriptEngineData
{
	// uuid, script object
	std::unordered_map<MonoType*, ScriptClass const*> types;
};

static ScriptEngineData _data;

constexpr static char const* INTERNAL_CLASS_NAME = "Runtime";

#define ADD_INTERNAL_CALL(csharpName, cppName) mono_add_internal_call(std::format("{}.{}::{}", ASSEMBLY_ENGINE_NAME, INTERNAL_CLASS_NAME, csharpName).c_str(), cppName)

#pragma region Util

static DefaultLayer& util_get_default_layer()
{
	return Application::instance().get_default_layer();
}

static SceneManager& util_get_scene_manager()
{
	return util_get_default_layer().get_scene_manager();
}

static Scene& util_get_scene()
{
	Ref<Scene> scene = ScriptEngine::instance().get_working_scene();
	MINTY_ASSERT(scene);
	return *scene;
}

static EntityRegistry& util_get_entity_registry()
{
	return util_get_scene().get_entity_registry();
}

static Window& util_get_window(UUID const id)
{
	Window* window = Window::get_window(id);
	MINTY_ASSERT(window);
	return *window;
}

static Window& util_get_main_window()
{
	return Window::main();
}

static Entity util_get_entity(UUID id)
{
	EntityRegistry& registry = util_get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	return entity;
}

static CameraComponent& util_get_camera_component(UUID id)
{
	MINTY_ASSERT(id.valid());

	EntityRegistry& registry = util_get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	CameraComponent* component = registry.try_get<CameraComponent>(entity);
	MINTY_ASSERT(component != nullptr);

	return *component;
}

static TransformComponent& util_get_transform_component(UUID id, bool setDirty)
{
	MINTY_ASSERT(id.valid());

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

#pragma region Console

static void console_log(MonoString* string)
{
	Console::log(ScriptEngine::from_mono_string(string));
}

static void console_log_color(MonoString* string, int color)
{
	Console::log_color(ScriptEngine::from_mono_string(string), static_cast<Console::Color>(color));
}

static void console_warn(MonoString* string)
{
	Console::warn(ScriptEngine::from_mono_string(string));
}

static void console_error(MonoString* string)
{
	Console::error(ScriptEngine::from_mono_string(string));
}

static void console_ass(bool condition, MonoString* string)
{
	Console::ass(condition, ScriptEngine::from_mono_string(string));
}

#pragma endregion

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

#pragma region Object

static void object_destroy_entity(UUID id)
{
	EntityRegistry& registry = util_get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	registry.destroy(entity);
}

static void object_destroy_immediate_entity(UUID id)
{
	EntityRegistry& registry = util_get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	registry.destroy_immediate(entity);
}

#pragma endregion

#pragma region Entity

static MonoString* entity_get_name(UUID id)
{
	if (!id) return ScriptEngine::instance().to_mono_string("");

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the name
	String name = registry.get_name(entity);

	// convert to mono string and return
	return ScriptEngine::instance().to_mono_string(name);
}

static void entity_set_name(UUID id, MonoString* string)
{
	if (!id) return;

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// set the name
	String name = ScriptEngine::from_mono_string(string);
	registry.set_name(entity, name);
}

static MonoString* entity_get_tag(UUID id)
{
	if (!id) return ScriptEngine::instance().to_mono_string("");

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the name
	String name = registry.get_tag(entity);

	// convert to mono string and return
	return ScriptEngine::instance().to_mono_string(name);
}

static void entity_set_tag(UUID id, MonoString* string)
{
	if (!id) return;

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// set the name
	String name = ScriptEngine::from_mono_string(string);
	registry.set_tag(entity, name);
}

static void entity_set_enabled(UUID id, bool enabled)
{
	MINTY_ASSERT(id.valid());

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

static bool entity_get_enabled(UUID id)
{
	MINTY_ASSERT(id.valid());

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	return registry.all_of<EnabledComponent>(entity);
}

static MonoObject* entity_add_component(UUID id, MonoReflectionType* reflectionType)
{
	MINTY_ASSERT(id.valid());

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the component name
	MonoType* type = mono_reflection_type_get_type(reflectionType);
	auto found = _data.types.find(type);
	if (found == _data.types.end())
	{
		MINTY_ABORT(std::format("{} cannot be added, it has not been registered.", mono_type_get_name(type)));
		return nullptr;
	}
	ScriptClass const* scriptClass = found->second;

	// get the component by name
	ScriptObjectComponent* component = static_cast<ScriptObjectComponent*>(registry.get_by_name(scriptClass->get_name(), entity));

	// if null, add the component
	if (!component)
	{
		component = static_cast<ScriptObjectComponent*>(registry.emplace_by_name(scriptClass->get_name(), entity));
	}

	// create a new object for this component, if needed
	ScriptObject const& componentObject = ScriptEngine::instance().get_or_create_component(component->id, id, *scriptClass);
	return componentObject.data();
}

static MonoObject* entity_get_component(UUID id, MonoReflectionType* reflectionType)
{
	MINTY_ASSERT(id.valid());

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the component name
	MonoType* type = mono_reflection_type_get_type(reflectionType);
	auto found = _data.types.find(type);
	if (found == _data.types.end())
	{
		MINTY_ABORT(std::format("{} cannot be gotten, it has not been registered.", mono_type_get_name(type)));
		// type not found
		return nullptr;
	}
	ScriptClass const* scriptClass = found->second;

	// get the component by name
	ScriptObjectComponent* component = static_cast<ScriptObjectComponent*>(registry.get_by_name(scriptClass->get_name(), entity));

	// if null, return null
	if (!component)
	{
		return nullptr;
	}

	// create a new object for this component
	ScriptObject const& componentObject = ScriptEngine::instance().get_or_create_component(component->id, id, *scriptClass);
	return componentObject.data();
}

static void entity_remove_component(UUID id, MonoReflectionType* reflectionType)
{
	MINTY_ASSERT(id.valid());

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the component name
	MonoType* type = mono_reflection_type_get_type(reflectionType);
	auto found = _data.types.find(type);
	if (found == _data.types.end())
	{
		MINTY_ABORT(std::format("{} cannot be removed, it has not been registered.", mono_type_get_name(type)));
		// type not found
		return;
	}
	ScriptClass const* scriptClass = found->second;

	// destroy the component by name
	registry.destroy(entity, scriptClass->get_name());
}

static MonoObject* entity_get_parent(UUID id)
{
	MINTY_ASSERT(id.valid());

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the parent
	Entity parent = registry.get_parent(entity);

	// get the ID
	UUID parentId = registry.get_id(parent);

	if (!parentId.valid()) return nullptr;

	return ScriptEngine::instance().get_or_create_entity(parentId).data();
}

static void entity_set_parent(UUID id, UUID parentId)
{
	MINTY_ASSERT(id.valid());

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the parent
	Entity parentEntity = registry.find_by_id(parentId);

	// set the parent
	registry.set_parent(entity, parentEntity);
}

static int entity_get_child_count(UUID id)
{
	MINTY_ASSERT(id.valid());

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	size_t childCount = registry.get_child_count(entity);

	MINTY_ASSERT_MESSAGE(childCount <= INT_MAX, "Entity child count exceeded INT_MAX.");

	return static_cast<int>(childCount);
}

static MonoObject* entity_get_child(UUID id, int index)
{
	MINTY_ASSERT(id.valid());

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// get the child
	Entity child = registry.get_child(entity, index);

	UUID childId = registry.get_id(child);

	if (!childId.valid()) return nullptr;

	return ScriptEngine::instance().get_or_create_entity(childId).data();
}

static MonoObject* entity_clone(UUID id)
{
	MINTY_ASSERT(id.valid());

	EntityRegistry& registry = util_get_entity_registry();

	// get the entity
	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);

	// clone it
	Entity clone = registry.clone(entity);

	// get the ID
	UUID cloneId = registry.get_id(clone);

	if (!cloneId.valid()) return nullptr;

	return ScriptEngine::instance().get_or_create_entity(cloneId).data();
}

#pragma endregion

#pragma region Window

static MonoObject* window_get(UUID id)
{
	ScriptEngine& scripts = ScriptEngine::instance();
	return scripts.get_object(id)->data();
}

static MonoObject* window_get_main()
{
	return window_get(Window::main().get_id());
}

static MonoString* window_get_title(UUID id)
{
	return ScriptEngine::instance().to_mono_string(util_get_window(id).get_title());
}

static void window_set_title(UUID id, MonoString* string)
{
	util_get_window(id).set_title(ScriptEngine::instance().from_mono_string(string));
}

static void window_set_icon(UUID id, MonoString* string)
{
	util_get_window(id).set_icon(ScriptEngine::instance().from_mono_string(string));
}

static bool window_is_open(UUID id)
{
	return util_get_window(id).is_open();
}

static void window_close(UUID id)
{
	util_get_window(id).close();
}

static void window_maximize(UUID id)
{
	util_get_window(id).maximize();
}

static void window_minimize(UUID id)
{
	util_get_window(id).minimize();
}

static void window_restore(UUID id)
{
	util_get_window(id).restore();
}

#pragma endregion

#pragma region Components

#pragma region Camera

static int camera_get_perspective(UUID id)
{
	CameraComponent& camera = util_get_camera_component(id);

	return static_cast<int>(camera.camera.get_perspective());
}

static void camera_set_perspective(UUID id, int value)
{
	CameraComponent& camera = util_get_camera_component(id);

	camera.camera.set_perspective(static_cast<Perspective>(value));
}

static float camera_get_fov(UUID id)
{
	CameraComponent& camera = util_get_camera_component(id);

	return camera.camera.get_fov();
}

static void camera_set_fov(UUID id, float value)
{
	CameraComponent& camera = util_get_camera_component(id);

	camera.camera.set_fov(value);
}

static float camera_get_near(UUID id)
{
	CameraComponent& camera = util_get_camera_component(id);

	return camera.camera.get_near();
}

static void camera_set_near(UUID id, float value)
{
	CameraComponent& camera = util_get_camera_component(id);

	camera.camera.set_near(value);
}

static float camera_get_far(UUID id)
{
	CameraComponent& camera = util_get_camera_component(id);

	return camera.camera.get_far();
}

static void camera_set_far(UUID id, float value)
{
	CameraComponent& camera = util_get_camera_component(id);

	camera.camera.set_far(value);
}

static void camera_set_as_main(UUID id)
{
	MINTY_ASSERT(id.valid());

	Scene& scene = util_get_scene();
	EntityRegistry& registry = scene.get_entity_registry();

	Entity entity = registry.find_by_id(id);
	MINTY_ASSERT(entity != NULL_ENTITY);
	MINTY_ASSERT(registry.all_of<CameraComponent>(entity));

	// set main in render system
	RenderSystem* renderSystem = scene.get_system_registry().find<RenderSystem>();
	MINTY_ASSERT(renderSystem != nullptr);

	renderSystem->set_camera(entity);
}

static Color::color_t camera_get_color(UUID id)
{
	CameraComponent& camera = util_get_camera_component(id);

	return static_cast<Color::color_t>(camera.camera.get_color());
}

static void camera_set_color(UUID id, Color::color_t value)
{
	CameraComponent& camera = util_get_camera_component(id);

	camera.camera.set_color(Color(value));
}

#pragma endregion

#pragma region Transform

static void transform_get_local_position(UUID id, Vector3* position)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*position = component.localPosition;
}

static void transform_set_local_position(UUID id, Vector3* position)
{
	TransformComponent& component = util_get_transform_component(id, true);

	component.localPosition = *position;
}

static void transform_get_local_rotation(UUID id, Vector4* rotation)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*rotation = Vector4(component.localRotation.y, component.localRotation.z, component.localRotation.x, component.localRotation.w);
}

static void transform_set_local_rotation(UUID id, Vector4* rotation)
{
	TransformComponent& component = util_get_transform_component(id, true);

	component.localRotation = Quaternion(rotation->w, rotation->y, rotation->z, rotation->x);
}

static void transform_get_local_scale(UUID id, Vector3* scale)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*scale = component.localScale;
}

static void transform_set_local_scale(UUID id, Vector3* scale)
{
	TransformComponent& component = util_get_transform_component(id, true);

	component.localScale = *scale;
}

static void transform_get_right(UUID id, Vector3* direction)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*direction = component.get_right();
}

static void transform_get_up(UUID id, Vector3* direction)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*direction = component.get_up();
}

static void transform_get_forward(UUID id, Vector3* direction)
{
	TransformComponent& component = util_get_transform_component(id, false);

	*direction = component.get_forward();
}

#pragma endregion

#pragma endregion

void Minty::ScriptLink::link()
{
	// link all the classes
	// systems
	register_system<AnimationSystem>("Animation");
	register_system<AudioSystem>("Audio");
	register_system<RenderSystem>("Render");
	register_system<ScriptSystem>("Script");
	register_system<UISystem>("UI");

	// components
	// ignore: Dirty, Destroy, etc. These are temporary components that should not be saved
	register_component<AnimatorComponent>(ASSEMBLY_ENGINE_NAME, "Animator", true);
	register_component<AudioListenerComponent>(ASSEMBLY_ENGINE_NAME, "AudioListener", true);
	register_component<AudioSourceComponent>(ASSEMBLY_ENGINE_NAME, "AudioSource", true);
	register_component<CameraComponent>(ASSEMBLY_ENGINE_NAME, "Camera", true);
	register_component<CanvasComponent>(ASSEMBLY_ENGINE_NAME, "Canvas", true);
	register_component<EnabledComponent>(ASSEMBLY_ENGINE_NAME, "Enabled", false);
	register_component<MeshComponent>(ASSEMBLY_ENGINE_NAME, "Mesh", true);
	register_component<NameComponent>(ASSEMBLY_ENGINE_NAME, "Name", true);
	register_component<RelationshipComponent>(ASSEMBLY_ENGINE_NAME, "Relationship", true);
	register_component<RenderableComponent>(ASSEMBLY_ENGINE_NAME, "Renderable", true);
	register_component<ScriptComponent>(ASSEMBLY_ENGINE_NAME, "Script", true);
	register_component<SpriteComponent>(ASSEMBLY_ENGINE_NAME, "Sprite", true);
	register_component<TransformComponent>(ASSEMBLY_ENGINE_NAME, "Transform", true);
	register_component<UITransformComponent>(ASSEMBLY_ENGINE_NAME, "UITransform", true);

	// link all the functions
#pragma region Time
	ADD_INTERNAL_CALL("Time_GetTotalTime", time_get_total);
	ADD_INTERNAL_CALL("Time_GetElapsedTime", time_get_elapsed);
#pragma endregion

#pragma region Console
	ADD_INTERNAL_CALL("Console_Log", console_log);
	ADD_INTERNAL_CALL("Console_LogColor", console_log_color);
	ADD_INTERNAL_CALL("Console_Warn", console_warn);
	ADD_INTERNAL_CALL("Console_Error", console_error);
	ADD_INTERNAL_CALL("Console_Assert", console_ass);
#pragma endregion

#pragma region Cursor
	ADD_INTERNAL_CALL("Cursor_GetMode", cursor_get_mode);
	ADD_INTERNAL_CALL("Cursor_SetMode", cursor_set_mode);
#pragma endregion

#pragma region Object
	ADD_INTERNAL_CALL("Object_DestroyEntity", object_destroy_entity);
	ADD_INTERNAL_CALL("Object_DestroyImmediateEntity", object_destroy_immediate_entity);
#pragma endregion

#pragma region Entity
	ADD_INTERNAL_CALL("Entity_GetName", entity_get_name);
	ADD_INTERNAL_CALL("Entity_SetName", entity_set_name);
	ADD_INTERNAL_CALL("Entity_GetTag", entity_get_tag);
	ADD_INTERNAL_CALL("Entity_SetTag", entity_set_tag);
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

#pragma region Window
	ADD_INTERNAL_CALL("Window_GetMain", window_get_main);
	ADD_INTERNAL_CALL("Window_Get", window_get);
	ADD_INTERNAL_CALL("Window_GetTitle", window_get_title);
	ADD_INTERNAL_CALL("Window_SetTitle", window_set_title);
	ADD_INTERNAL_CALL("Window_SetIcon", window_set_icon);
	ADD_INTERNAL_CALL("Window_IsOpen", window_is_open);
	ADD_INTERNAL_CALL("Window_Close", window_close);
	ADD_INTERNAL_CALL("Window_Maximize", window_maximize);
	ADD_INTERNAL_CALL("Window_Minimize", window_minimize);
	ADD_INTERNAL_CALL("Window_Restore", window_restore);
#pragma endregion

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
	ADD_INTERNAL_CALL("Camera_SetAsMain", camera_set_as_main);
	ADD_INTERNAL_CALL("Camera_GetColor", camera_get_color);
	ADD_INTERNAL_CALL("Camera_SetColor", camera_set_color);
#pragma endregion

#pragma region Transform
	ADD_INTERNAL_CALL("Transform_GetLocalPosition", transform_get_local_position);
	ADD_INTERNAL_CALL("Transform_SetLocalPosition", transform_set_local_position);
	ADD_INTERNAL_CALL("Transform_GetLocalRotation", transform_get_local_rotation);
	ADD_INTERNAL_CALL("Transform_SetLocalRotation", transform_set_local_rotation);
	ADD_INTERNAL_CALL("Transform_GetLocalScale", transform_get_local_scale);
	ADD_INTERNAL_CALL("Transform_SetLocalScale", transform_set_local_scale);
	ADD_INTERNAL_CALL("Transform_GetRight", transform_get_right);
	ADD_INTERNAL_CALL("Transform_GetUp", transform_get_up);
	ADD_INTERNAL_CALL("Transform_GetForward", transform_get_forward);
#pragma endregion

#pragma endregion
}

#undef ADD_INTERNAL_CALL

void Minty::ScriptLink::link_script(String const& fullName)
{
	ScriptEngine& engine = ScriptEngine::instance();

	// find ScriptClass with the given name
	ScriptClass const* script = engine.find_class(fullName);

	// ignore if no script found
	if (!script) return;

	// if found, register it
	link_script(*script);
}

void Minty::ScriptLink::link_script(ScriptClass const& script)
{
	MonoType* type = script.get_type();

	MINTY_ASSERT(type != nullptr);

	_data.types[type] = &script;

	MINTY_INFO_FORMAT("Linked {}.", script.get_full_name());
}
