#include "pch.h"
#include "M_AnimationSystem.h"

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

	for (auto&& [entity, animatorComponent] : get_entity_registry().view<AnimatorComponent>().each())
	{
		//// skip if no animation
		//if (animatorComponent.animationId == ERROR_ID)
		//{
		//	continue;
		//}

		//// update the animator, get the current animation ID
		//Animator& animator = get_animator(animatorComponent.animatorId);

		//ID newId = animator.update();

		//// if the animation has not changed

		//// elapse time
		//animatorComponent.time -= deltaTime;

		//// if time is over, move to next animation stage
		//if (animatorComponent.time > 0.0f)
		//{
		//	// do nothing, keep rendering current frame
		//	continue;
		//}

		//// frame over: move to the next frame
		//animatorComponent.index++;

		//Animation const& animation = _animations.at(animatorComponent.animationId);

		//// if still on animation, reset frame time and move on
		//if (animatorComponent.index < animation.get_frame_count())
		//{
		//	update_animation(animatorComponent, animation);

		//	continue;
		//}

		//// animation over: move to the next animation

		//// evaluate the animator, get the new animation ID, if any change at all
		//animatorComponent.animationId = animator.update();

		//// go to first frame
		//animatorComponent.index = 0;

		//// set sprite id and time
		//update_animation(animatorComponent, animation);
	}
}

void minty::AnimationSystem::reset()
{
	_animations.clear();
	_animators.clear();
}

void minty::AnimationSystem::update_animation(AnimatorComponent& animatorComponent, Animation const& animation) const
{
	//// set time
	//float frameTime = animation.get_frame_time();
	//animatorComponent.time = math::mod(animatorComponent.time, frameTime) + frameTime;

	//// set frame
	//SpriteComponent& spriteComponent = get_entity_registry().get<SpriteComponent>(animatorComponent.entity);
	//spriteComponent.spriteId = animation.get_frame(animatorComponent.index);
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

	AnimationBuilder builder
	{
		.name = reader.read_string("name", meta.to_string()),
		.length = reader.read_float("length"),
	};
	reader.read_vector("entities", builder.entities);
	reader.read_vector("components", builder.components);
	reader.read_vector("sizes", builder.sizes);
	reader.read_vector("values", builder.values);

	// get renderer
	RenderSystem* renderer = get_scene().get_system_registry().find<RenderSystem>();

	MINTY_ASSERT(renderer != nullptr, "AnimationSystem::load_animation(): renderer must not be null.");

	//// get sprite IDs from loaded names
	//std::vector<String> names;
	//reader.read_vector("frames", names);
	//builder.frames.resize(names.size());
	//for (size_t i = 0; i < names.size(); i++)
	//{
	//	builder.frames[i] = renderer->find_sprite(names.at(i));
	//}

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
	reader.read_object("fsm", builder.fsm);

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
