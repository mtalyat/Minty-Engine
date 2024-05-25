#include "pch.h"
#include "Minty/Animation/M_AnimationSystem.h"

#include "Minty/Animation/M_AnimatorComponent.h"
#include "Minty/Rendering/M_SpriteComponent.h"
#include "Minty/Components/M_EnabledComponent.h"
#include "Minty/Assets/M_Asset.h"
#include "Minty/Rendering/M_RenderEngine.h"
#include "Minty/Rendering/M_RenderSystem.h"
#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Entities/M_EntityRegistry.h"
#include "Minty/Systems/M_SystemRegistry.h"
#include "Minty/Scenes/M_Scene.h"
#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

using namespace Minty;

void Minty::AnimationSystem::update(Time const time)
{
	float deltaTime = time.elapsed;

	EntityRegistry& registry = get_entity_registry();

	AssetEngine& assets = AssetEngine::instance();

	for (auto&& [entity, animatorComponent, enabled] : registry.view<AnimatorComponent, EnabledComponent const>().each())
	{
		// update the animator, get the current animation ID
		UUID newId = animatorComponent.animator.update();
		Ref<Animation> newAnimation = assets.get<Animation>(newId);

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
