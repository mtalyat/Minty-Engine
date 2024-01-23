#pragma once

#include "M_Object.h"

namespace minty
{
	class AnimationSystem;

	class AnimationObject
		: public Object
	{
	private:
		AnimationSystem* _animationSystem;

	public:
		AnimationObject();

		AnimationObject(AnimationSystem& system);

		AnimationSystem* get_animation_system();

		AnimationSystem const* get_animation_system() const;

		virtual ~AnimationObject();
	};
}