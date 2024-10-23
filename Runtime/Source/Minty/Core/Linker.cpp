#include "pch.h"
#include "Linker.h"

#include "Minty/Component/AllComponents.h"
#include "Minty/Core/Application.h"
#include "Minty/Core/Constants.h"
#include "Minty/System/AllSystems.h"
#include "Minty/Script/CS/CsLinker.h"

Minty::Linker::Data Minty::Linker::s_data = {};

using namespace Minty;

void Minty::Linker::link()
{
	// systems
	register_system<AnimationSystem>("Animation");
	//register_system<AudioSystem>("Audio");
	register_system<RenderSystem>("Render");
	register_system<ScriptSystem>("Script");
	//register_system<UISystem>("UI");

	// components
	// ignore: Dirty, Destroy, etc. These are temporary components that should not be saved
	register_component<AnimatorComponent>(MINTY_NAME_ENGINE, "Animator", true);
	//register_component<AudioListenerComponent>(MINTY_NAME_ENGINE, "AudioListener", true);
	//register_component<AudioSourceComponent>(MINTY_NAME_ENGINE, "AudioSource", true);
	register_component<CameraComponent>(MINTY_NAME_ENGINE, "Camera", true);
	register_component<CanvasComponent>(MINTY_NAME_ENGINE, "Canvas", true);
	register_component<EnabledComponent>(MINTY_NAME_ENGINE, "Enabled", false);
	register_component<MeshComponent>(MINTY_NAME_ENGINE, "Mesh", true);
	register_component<NameComponent>(MINTY_NAME_ENGINE, "Name", true);
	register_component<RelationshipComponent>(MINTY_NAME_ENGINE, "Relationship", true);
	register_component<RenderableComponent>(MINTY_NAME_ENGINE, "Renderable", true);
	register_component<ScriptComponent>(MINTY_NAME_ENGINE, "Script", true);
	register_component<SpriteComponent>(MINTY_NAME_ENGINE, "Sprite", true);
	//register_component<TextComponent>(MINTY_NAME_ENGINE, "Text", true);
	register_component<TagComponent>(MINTY_NAME_ENGINE, "Tag", true);
	register_component<TransformComponent>(MINTY_NAME_ENGINE, "Transform", true);
	register_component<UITransformComponent>(MINTY_NAME_ENGINE, "UITransform", true);

	// link script specific
	CsLinker::link();
}

void Minty::Linker::link_script(Ref<ScriptClass> const script)
{
	MINTY_ASSERT_MESSAGE(script != nullptr, "Cannot link a null script.");

	String const& name = script->get_full_name();

	s_data.types.emplace(name, script);

	MINTY_INFO_FORMAT("Linked {}.", name);
}
