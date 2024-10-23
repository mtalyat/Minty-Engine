#include "pch.h"
#include "AnimationSystem.h"

#include "Minty/Component/AllComponents.h"
#include "Minty/Asset/Asset.h"
#include "Minty/Render/Renderer.h"
#include "Minty/System/RenderSystem.h"
#include "Minty/Asset/AssetManager.h"
#include "Minty/Scene/Scene.h"

using namespace Minty;

void Minty::AnimationSystem::update(Time const& time)
{
	float deltaTime = time.elapsed;

	Scene& scene = get_scene();
	EntityRegistry& registry = scene.get_entity_registry();

	for (auto&& [entity, animatorComponent, enabled] : registry.view<AnimatorComponent, EnabledComponent const>().each())
	{
		// update the animator, get the current animation ID
		UUID newId = animatorComponent.animator->update();
		Ref<Animation> newAnimation = AssetManager::get<Animation>(newId);

		// if the ID has changed, reset with new animation data
		if (animatorComponent.animation != newAnimation)
		{
			// reset animation state data

			// get the animation so it can be used
			if (animatorComponent.animation.get())
			{
				animatorComponent.animation->reset(entity, scene);
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
		if (animatorComponent.animation->animate(animatorComponent.time, deltaTime, animatorComponent.index, entity, scene))
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

		// assuming something changed that needs updating, dirty the entity
		registry.dirty(entity);
	}
}