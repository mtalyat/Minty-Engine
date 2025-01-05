#include "pch.h"
#include "TransformComponent.h"

using namespace Minty;

TransformComponent Minty::TransformComponent::create_empty()
{
	TransformComponent component{};
	component.globalMatrix = component.get_local_matrix();
	return component;
}

void Minty::TransformComponent::serialize(Writer& writer) const
{
	writer.write("position", localPosition);
	writer.write("rotation", to_euler(localRotation));
	writer.write("scale", localScale);
}

void Minty::TransformComponent::deserialize(Reader& reader)
{
	reader.read("position", localPosition);
	Float3 eulerAngles;
	if (reader.read("rotation", eulerAngles))
	{
		localRotation = from_euler(eulerAngles);
	}
	reader.read("scale", localScale);
}
