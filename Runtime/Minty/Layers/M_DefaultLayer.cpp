#include "pch.h"
#include "M_DefaultLayer.h"

#include "Minty/Core/M_Application.h"
#include "Minty/Events/M_Event.h"
#include "Minty/Events/M_KeyEvent.h"
#include "Minty/Events/M_MouseEvent.h"
#include "Minty/Events/M_GamepadEvent.h"
#include "Minty/Input/M_Input.h"

#include "Minty/Rendering/M_RenderEngine.h"

#include "Minty/Assets/M_AssetEngine.h"

#include "Minty/Scenes/M_Scene.h"
#include "Minty/UI/M_UITransformComponent.h"
#include "Minty/Entities/M_EntityRegistry.h"

#include "Minty/Serialization/M_Reader.h"

void Minty::DefaultLayer::on_attach()
{
	// TODO: find and load first scene, in a better way
	AssetEngine& assets = AssetEngine::instance();
	Path applicationDataPath = String("game").append(EXTENSION_APPLICATION_DATA);

	if (assets.exists(applicationDataPath))
	{
		Node node = assets.read_file_node(applicationDataPath);
		Reader reader(node);

		std::vector<Path> scenePaths;
		if (reader.try_read_vector("scenes", scenePaths))
		{
			// if no scenes listed, nothing can be ran
			if (scenePaths.empty())
			{
				MINTY_ABORT("No scenes found.");
				return;
			}

			// load the first scene
			Ref<Scene> startingScene = _sceneManager.create_scene(scenePaths.front());
			_sceneManager.load_scene(startingScene->get_id());
		}
	}

	_sceneManager.load();
}

void Minty::DefaultLayer::on_detach()
{
	_sceneManager.unload();
}

void Minty::DefaultLayer::on_update(Time const& time)
{
	// update
	_sceneManager.update(time);

	// late update
	_sceneManager.finalize();

	// render to the screen
	RenderEngine::instance().render_frame();
}

void Minty::DefaultLayer::on_event(Event& event)
{
	// these events are always called
	switch (event.get_event_type())
	{
	case EventType::WindowResize:
	{
		// TODO: move this out of this function into more natural place

		Ref<Scene> scene = _sceneManager.get_working_scene();
		if (!scene) break;

		// resize UI elements
		EntityRegistry& registry = scene->get_entity_registry();

		for (auto [entity, transform] : registry.view<UITransformComponent>().each())
		{
			registry.dirty(entity);
		}

		break;
	}
	}

	// these events only called in normal run mode
	if (Application::instance().get_mode() == ApplicationMode::Normal)
	{
		switch (event.get_event_type())
		{
		case EventType::Key:
		{
			KeyEvent& e = static_cast<KeyEvent&>(event);
			Input::trigger_key(e.get_key(), e.get_key_action(), e.get_key_modifiers());
			break;
		}
		case EventType::MouseButton:
		{
			MouseButtonEvent& e = static_cast<MouseButtonEvent&>(event);
			Input::trigger_mouse_button(e.get_button(), e.get_action(), e.get_key_modifiers());
			break;
		}
		case EventType::MouseMoved:
		{
			MouseMovedEvent& e = static_cast<MouseMovedEvent&>(event);
			Input::trigger_mouse_move(e.get_x(), e.get_y());
			break;
		}
		case EventType::MouseScrolled:
		{
			MouseScrolledEvent& e = static_cast<MouseScrolledEvent&>(event);
			Input::trigger_mouse_scroll(e.get_delta_x(), e.get_delta_y());
			break;
		}
		case EventType::GamepadConnected:
		{
			GamepadConnectedEvent& e = static_cast<GamepadConnectedEvent&>(event);
			Input::trigger_gamepad_connect(e.get_controller());
			break;
		}
		case EventType::GamepadDisconnected:
		{
			GamepadDisconnectedEvent& e = static_cast<GamepadDisconnectedEvent&>(event);
			Input::trigger_gamepad_disconnect(e.get_controller());
			break;
		}
		case EventType::GamepadButton:
		{
			GamepadButtonEvent& e = static_cast<GamepadButtonEvent&>(event);
			Input::trigger_gamepad_button(e.get_controller(), e.get_button(), e.get_action());
			break;
		}
		case EventType::GamepadAxis:
		{
			GamepadAxisEvent& e = static_cast<GamepadAxisEvent&>(event);
			Input::trigger_gamepad_axis(e.get_controller(), e.get_axis(), e.get_value());
			break;
		}
		}
	}
}
