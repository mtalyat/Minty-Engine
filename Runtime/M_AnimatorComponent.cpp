#include "pch.h"
#include "M_AnimatorComponent.h"

using namespace minty;

void minty::AnimatorComponent::serialize(Writer& writer) const
{
	console::todo("AnimatorComponent::serialize()");
}

void minty::AnimatorComponent::deserialize(Reader const& reader)
{
	console::todo("AnimatorComponent::deserialize()");
}

String minty::to_string(AnimatorComponent const& value)
{
	return std::format("AnimatorComponent()");
}
