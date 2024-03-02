#include "pch.h"
#include "M_AnimationSystem.h"

#include "M_AnimatorComponent.h"
#include "M_SpriteComponent.h"
#include "M_EnabledComponent.h"
#include "M_Asset.h"
#include "M_RenderEngine.h"
#include "M_RenderSystem.h"
#include "M_Runtime.h"
#include "M_AssetEngine.h"
#include "M_EntityRegistry.h"
#include "M_SystemRegistry.h"
#include "M_Scene.h"
#include "M_Reader.h"
#include "M_Writer.h"

using namespace minty;

minty::AnimationSystem::AnimationSystem(Runtime& engine, Scene& scene)
	: System("Animation", engine, scene)
{}

void minty::AnimationSystem::update()
{
	float deltaTime = get_scene().get_runtime().get_time().elapsed;

	EntityRegistry& registry = get_entity_registry();

	AssetEngine& assets = get_runtime().get_asset_engine();

	for (auto&& [entity, animatorComponent, enabled] : registry.view<AnimatorComponent, EnabledComponent const>().each())
	{
		// update the animator, get the current animation ID
		UUID newId = animatorComponent.animator.update();
		Animation* newAnimation = assets.get<Animation>(newId);

		// if the ID has changed, reset with new animation data
		if (animatorComponent.animation != newAnimation)
		{
			// reset animation state data
			
			// get the animation so it can be used
			if (animatorComponent.animation)
			{
				animatorComponent.animation->reset(entity, get_scene());
			}

			// reset animator for new animation
			animatorComponent.animation = newAnimation;
			animatorComponent.reset();
		}

		// if the animator time is below zero, then the animator has paused, so do nothing
		// OR if the animation ID is ERROR_ID, do nothing
		if (animatorComponent.time < 0.0f || !animatorComponent.animation)
		{
			continue;
		}

		// animate with it
		if (animatorComponent.animation->animate(animatorComponent.time, deltaTime, animatorComponent.index, entity, get_scene()))
		{
			// animation has completed, loop if supposed to
			if (animatorComponent.animation->loops())
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
{}

void minty::AnimationSystem::serialize(Writer& writer) const
{
	Console::todo("AnimationSystem::serialize()");
}

void minty::AnimationSystem::deserialize(Reader const& reader)
{
	register_assets(reader, "animations", [](AssetEngine& assets, Path const& path) { return assets.load_animation(path); });
	register_assets(reader, "animators", [](AssetEngine& assets, Path const& path) { return assets.load_animator(path); });
}
