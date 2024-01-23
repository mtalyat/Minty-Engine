#include "pch.h"
#include "M_AnimationObject.h"

using namespace minty;

minty::AnimationObject::AnimationObject()
	:_animationSystem()
{}

minty::AnimationObject::AnimationObject(AnimationSystem& system)
	:_animationSystem(&system)
{}

AnimationSystem* minty::AnimationObject::get_animation_system()
{
	return _animationSystem;
}

AnimationSystem const* minty::AnimationObject::get_animation_system() const
{
	return _animationSystem;
}

minty::AnimationObject::~AnimationObject()
{}
