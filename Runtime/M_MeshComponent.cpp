#include "pch.h"
#include "M_MeshComponent.h"

#include "M_Console.h"

using namespace minty;

void minty::MeshComponent::serialize(Writer& writer) const
{
	console::todo("MeshComponent::serialize");
}

void minty::MeshComponent::deserialize(Reader const& reader)
{
	console::todo("MeshComponent::deserialize");
}

std::string minty::to_string(MeshComponent const& value)
{
	return std::format("MeshComponent()");
}
