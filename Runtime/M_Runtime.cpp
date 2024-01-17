#include "pch.h"
#include "M_Runtime.h"

#include "M_SystemRegistry.h"
#include "M_AnimationSystem.h"
#include "M_UISystem.h"

#include "M_EntityRegistry.h"
#include "M_AnimatorComponent.h"
#include "M_AudioListenerComponent.h"
#include "M_AudioSourceComponent.h"
#include "M_CameraComponent.h"
#include "M_DirtyComponent.h"
#include "M_MeshComponent.h"
#include "M_NameComponent.h"
#include "M_RelationshipComponent.h"
#include "M_RenderableComponent.h"
#include "M_SpriteComponent.h"
#include "M_TransformComponent.h"
#include "M_UITransformComponent.h"

#include "M_CommandLineParser.h"
#include "M_Console.h"
#include <filesystem>
#include <iostream>
#include <exception>
#include <typeinfo>
#include <stdexcept>
#include <exception>

using namespace minty;

minty::Runtime::Runtime(Info const& appInfo)
	: _engine(appInfo)
{
	register_builtin();
}

Runtime::~Runtime()
{}

Engine& minty::Runtime::get_engine()
{
	return _engine;
}

Engine const& minty::Runtime::get_engine() const
{
	return _engine;
}

int Runtime::run()
{
	try
	{
		_engine.run();
	}
	catch (std::exception const& e)
	{
		console::error(std::format("Program aborted: \"{}\"", e.what()));
		return EXIT_FAILURE;
	}
	catch (...)
	{
		console::error("An uncaught exception occured.");
	}

	return EXIT_SUCCESS;
}

void minty::Runtime::register_builtin()
{
	// systems
	SystemRegistry::register_system<AnimationSystem>("Animation");
	SystemRegistry::register_system<UISystem>("UI");

	// components
	EntityRegistry::register_component<AnimatorComponent>("Animator");
	EntityRegistry::register_component<AudioListenerComponent>("AudioListener");
	EntityRegistry::register_component<AudioSourceComponent>("AudioSource");
	EntityRegistry::register_component<CameraComponent>("Camera");
	EntityRegistry::register_component<DirtyComponent>("Dirty");
	EntityRegistry::register_component<MeshComponent>("Mesh");
	EntityRegistry::register_component<NameComponent>("Name");
	EntityRegistry::register_component<RelationshipComponent>("Relationship");
	EntityRegistry::register_component<RenderableComponent>("Renderable");
	EntityRegistry::register_component<SpriteComponent>("Sprite");
	EntityRegistry::register_component<TransformComponent>("Transform");
	EntityRegistry::register_component<UITransformComponent>("UITransform");
}

String minty::to_string(Runtime const& runtime)
{
	return std::format("Runtime()");
}
