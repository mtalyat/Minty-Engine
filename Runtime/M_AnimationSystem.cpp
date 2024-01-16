#include "pch.h"
#include "M_AnimationSystem.h"

#include "M_SpriteComponent.h"
#include "M_Asset.h"
#include "M_RenderEngine.h"
#include "M_Engine.h"

using namespace minty;

minty::AnimationSystem::AnimationSystem(minty::Engine* const engine, minty::EntityRegistry* const registry)
	: System(engine, registry)
{}

void minty::AnimationSystem::update()
{
	for (auto&& [entity, animator] : _registry->view<AnimatorComponent>().each())
	{
		// evaluate the animator
		// if the animator is at a new state, update the SpriteComponent sprite id
		if (int error = animator.fsm.evaluate())
		{
			// an error occured
			switch (error)
			{
			case 1:
				// just didn't transition
				break;
			case 2:
				// no current state
				break;
			case 3:
				// infinite loop
				console::warn(std::format("Infinite loop indicated in AnimatorComponent for entity \"{}\".", _registry->get_name(entity)));
				break;
			default:
				console::error(std::format("Unrecognized FSM error code in AnimationSystem: {}", error));
				break;
			}

			// skip this entity
			continue;
		}

		// get new sprite ID, set it to the sprite component
		SpriteComponent& spriteComponent = _registry->get<SpriteComponent>(animator.entity);
		spriteComponent.spriteId = animator.fsm.get_current_value();
	}
}

ID minty::AnimationSystem::create_animation(AnimationBuilder const& builder)
{
	return _animations.emplace(builder.name, builder);
}

ID minty::AnimationSystem::find_animation(String const& name) const
{
	return _animations.find(name);
}

Animation& minty::AnimationSystem::get_animation(ID const id)
{
	return _animations.at(id);
}

Animation const& minty::AnimationSystem::get_animation(ID const id) const
{
	return _animations.at(id);
}

String minty::AnimationSystem::get_animation_name(ID const id) const
{
	return _animations.get_name(id);
}

ID minty::AnimationSystem::load_animation(Path const& path)
{
	if (Asset::check(path, false))
	{
		return ERROR_ID;
	}

	Node meta = Asset::load_node(path);

	AnimationBuilder builder
	{
		.name = meta.get_string("name", meta.to_string()),
		.frameTime = meta.get_float("frameTime", 0.25f),
	};

	// get renderer
	RenderEngine& renderer = _engine->get_render_engine();

	// get sprite IDs from loaded names
	std::vector<String> names = meta.get_vector_string("frames");
	builder.frames.resize(names.size());
	for (size_t i = 0; i < names.size(); i++)
	{
		builder.frames[i] = renderer.find_sprite(names.at(i));
	}

	return create_animation(builder);
}

void minty::AnimationSystem::destroy_animation(ID const id)
{
	if (_animations.contains(id))
	{
		// just erase
		_animations.erase(id);
	}
}
