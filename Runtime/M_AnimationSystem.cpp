#include "pch.h"
#include "M_AnimationSystem.h"

#include "M_AnimationBuilder.h"
#include "M_SpriteComponent.h"
#include "M_Asset.h"
#include "M_RenderEngine.h"
#include "M_RenderSystem.h"
#include "M_Engine.h"

using namespace minty;

minty::AnimationSystem::AnimationSystem(Engine& engine, ID const sceneId)
	: System(engine, sceneId)
	, _animators()
	, _animations()
{}

void minty::AnimationSystem::update()
{
	float deltaTime = get_scene().get_engine().get_delta_time();

	EntityRegistry& registry = get_entity_registry();

	for (auto&& [entity, animatorComponent] : registry.view<AnimatorComponent>().each())
	{
		// update the animator, get the current animation ID
		ID newId = animatorComponent.animator.update();

		// if the ID has changed, reset with new animation data
		if (animatorComponent.animationId != newId)
		{
			animatorComponent.animationId = newId;
			animatorComponent.reset();
		}

		// skip if no animation
		if (animatorComponent.animationId == ERROR_ID)
		{
			continue;
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
		if (animation.animate(animatorComponent.time, deltaTime, animatorComponent.index, entity, registry))
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
	return _animations.at(id);
}

Animation const& minty::AnimationSystem::get_animation(ID const id) const
{
	return _animations.at(id);
}

Animator& minty::AnimationSystem::get_animator(ID const id)
{
	return _animators.at(id);
}

Animator const& minty::AnimationSystem::get_animator(ID const id) const
{
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
	if (Asset::check(path, false))
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
	reader.read_vector("sizes", builder.sizes);
	reader.read_vector("values", builder.values);

	RenderSystem* renderSystem = get_system_registry().find<RenderSystem>();

	// if the values are strings, parse them
	for (Dynamic& value : builder.values)
	{
		String valueString = value.get_string();

		if (valueString.starts_with("Sprite: "))
		{
			ID id = renderSystem->find_sprite(valueString.substr(8, valueString.size() - 8));
			value.set(&id);
		}
		else if (valueString.starts_with("Texture: "))
		{
			ID id = renderSystem->find_texture(valueString.substr(8, valueString.size() - 8));
			value.set(&id);
		}

		// else, leave be
	}

	// read the steps, but split them up by '/'
	if (Node const* stepsNode = reader.get_node().find("steps"))
	{
		// for each child, that is a time stamp
		// each child of each time stamp are the actual steps
		// all are the node names
		for (Node const& timeStampNode : stepsNode->get_children())
		{
			float time = parse::to_float(timeStampNode.get_name());

			// get actual step values
			for (Node const& stepNode : timeStampNode.get_children())
			{
				// split the line by /s
				std::vector<String> split = string::split(stepNode.get_name(), '/');
				// create the step
				Animation::Step step
				{
					.type = (split.size() >= 1 && split.at(0).size()) ? static_cast<Animation::StepType>(parse::to_size(split.at(0))) : Animation::StepType::Ignore,
					.entityIndex = (split.size() >= 2 && split.at(1).size()) ? parse::to_size(split.at(1)) : Animation::MAX_INDEX,
					.componentIndex = (split.size() >= 3 && split.at(2).size()) ? parse::to_size(split.at(2)) : Animation::MAX_INDEX,
					.offsetIndex = (split.size() >= 4 && split.at(3).size()) ? parse::to_size(split.at(3)) : Animation::MAX_INDEX,
					.sizeIndex = (split.size() >= 5 && split.at(4).size()) ? parse::to_size(split.at(4)) : Animation::MAX_INDEX,
					.valueIndex = (split.size() >= 6 && split.at(5).size()) ? parse::to_size(split.at(5)) : Animation::MAX_VALUE_INDEX,
				};
				// add to builder
				builder.steps.push_back({ time, step });
			}
		}
	}

	return create_animation(builder);
}

ID minty::AnimationSystem::load_animator(Path const& path)
{
	if (Asset::check(path, false))
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
	console::todo("AnimationSystem::serialize()");
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
