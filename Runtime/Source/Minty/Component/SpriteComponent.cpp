#include "pch.h"
#include "SpriteComponent.h"

using namespace Minty;

void Minty::SpriteComponent::serialize(Writer& writer) const
{
	if (sprite.get())
	{
		writer.write("sprite", sprite->id());
	}
	writer.write("color", color);
}

void Minty::SpriteComponent::deserialize(Reader& reader)
{
	UUID temp;
	if (reader.read("sprite", temp))
	{
		sprite = AssetManager::get<Sprite>(temp);
	}
	reader.read("color", color);
}
