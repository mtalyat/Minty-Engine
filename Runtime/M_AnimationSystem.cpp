#include "pch.h"
#include "M_AnimationSystem.h"

#include "M_AnimatorComponent.h"
#include "M_AnimatorBuilder.h"
#include "M_AnimationBuilder.h"
#include "M_SpriteComponent.h"
#include "M_Asset.h"
#include "M_RenderEngine.h"
#include "M_RenderSystem.h"
#include "M_Runtime.h"
#include "M_EntityRegistry.h"
#include "M_SystemRegistry.h"
#include "M_Scene.h"

using namespace minty;

minty::AnimationSystem::AnimationSystem(Runtime& engine, ID const sceneId)
	: System(engine, sceneId)
	, _animators()
	, _animations()
{}

void minty::AnimationSystem::update()
{
	float deltaTime = get_scene().get_engine().get_time().elapsed;

	EntityRegistry& registry = get_entity_registry();

	for (auto&& [entity, animatorComponent] : registry.view<AnimatorComponent>().each())
	{
		// update the animator, get the current animation ID
		ID newId = animatorComponent.animator.update();

		// if the ID has changed, reset with new animation data
		if (animatorComponent.animationId != newId)
		{
			// reset animation state data
			
			// get the animation so it can be used
			if (animatorComponent.animationId != ERROR_ID)
			{
				Animation const& oldAnimation = _animations.at(animatorComponent.animationId);
				oldAnimation.reset(entity, get_scene());
			}

			// reset animator for new animation
			animatorComponent.animationId = newId;
			animatorComponent.reset();
		}

		// if the animator time is below zero, then the animator has paused, so do nothing
		// OR if the animation ID is ERROR_ID, do nothing
		if (animatorComponent.time < 0.0f || animatorComponent.animationId == ERROR_ID)
		{
			continue;
		}

		// get the animation so it can be used
		Animation const& animation = _animations.at(animatorComponent.animationId);

		// animate with it
		if (animation.animate(animatorComponent.time, deltaTime, animatorComponent.index, entity, get_scene()))
		{
			// animation has completed, loop if supposed to
			if (animation.loops())
			{
				animatorComponent.reset();
			}
			else
			{
				// not looping

				// set time to -1 to indicate that it has stopped, until a new animation ID is given
				animatorComponent.time = -1.0f;
			}
		}
	}
}

void minty::AnimationSystem::reset()
{
	_animations.clear();
	_animators.clear();
}

ID minty::AnimationSystem::create_animation(AnimationBuilder const& builder)
{
	return _animations.emplace(builder.name, builder);
}

ID minty::AnimationSystem::create_animator(AnimatorBuilder const& builder)
{
	return _animators.emplace(builder.name, builder);
}

ID minty::AnimationSystem::find_animation(String const& name) const
{
	return _animations.find(name);
}

ID minty::AnimationSystem::find_animator(String const& name) const
{
	return _animators.find(name);
}

Animation& minty::AnimationSystem::get_animation(ID const id)
{
	MINTY_ASSERT(_animations.contains(id), std::format("AnimationSystem::get_animation(): does not contain the ID {}.", id));

	return _animations.at(id);
}

Animation const& minty::AnimationSystem::get_animation(ID const id) const
{
	MINTY_ASSERT(_animations.contains(id), std::format("AnimationSystem::get_animation(): does not contain the ID {}.", id));

	return _animations.at(id);
}

Animator& minty::AnimationSystem::get_animator(ID const id)
{
	MINTY_ASSERT(_animators.contains(id), std::format("AnimationSystem::get_animator(): does not contain the ID {}.", id));

	return _animators.at(id);
}

Animator const& minty::AnimationSystem::get_animator(ID const id) const
{
	MINTY_ASSERT(_animators.contains(id), std::format("AnimationSystem::get_animator(): does not contain the ID {}.", id));

	return _animators.at(id);
}

String minty::AnimationSystem::get_animation_name(ID const id) const
{
	return _animations.get_name(id);
}

String minty::AnimationSystem::get_animator_name(ID const id) const
{
	return _animations.get_name(id);
}

ID minty::AnimationSystem::load_animation(Path const& path)
{
	if (Asset::check(path, ANIMATION_EXTENSION, false))
	{
		return ERROR_ID;
	}

	Node meta = Asset::load_node(path);
	Reader reader(meta);

	// read basic values
	AnimationBuilder builder
	{
		.name = reader.read_string("name", meta.to_string()),
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

	RenderSystem* renderSystem = get_system_registry().find<RenderSystem>();

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

	return create_animation(builder);
}

ID minty::AnimationSystem::load_animator(Path const& path)
{
	if (Asset::check(path, ANIMATOR_EXTENSION, false))
	{
		return ERROR_ID;
	}

	Node meta = Asset::load_node(path);
	Reader reader(meta);

	AnimatorBuilder builder
	{
		.name = reader.read_string("name", meta.to_string()),
	};
	reader.read_object_ref("fsm", builder.fsm);

	return create_animator(builder);
}

void minty::AnimationSystem::destroy_animation(ID const id)
{
	if (_animations.contains(id))
	{
		_animations.erase(id);
	}
}

void minty::AnimationSystem::destroy_animator(ID const id)
{
	if (_animators.contains(id))
	{
		_animators.erase(id);
	}
}

void minty::AnimationSystem::serialize(Writer& writer) const
{
	Console::todo("AnimationSystem::serialize()");
}

void minty::AnimationSystem::deserialize(Reader const& reader)
{
	reset();

	std::vector<Path> paths;
	reader.read_vector("animations", paths);
	for (Path const& path : paths)
	{
		load_animation(path);
	}
	reader.read_vector("animators", paths);
	for (Path const& path : paths)
	{
		load_animator(path);
	}
}
