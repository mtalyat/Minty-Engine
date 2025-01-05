#include "pch.h"
#include "UITransformComponent.h"

using namespace Minty;

void Minty::UITransformComponent::update_global_rect(Rect const parentRect)
{
	if ((anchorMode & AnchorMode::Left) != AnchorMode::Empty)
	{
		globalRect.x = parentRect.x + x;
		globalRect.width = width;
	}
	else if ((anchorMode & AnchorMode::Center) != AnchorMode::Empty)
	{
		globalRect.x = parentRect.x + x + (parentRect.width * 0.5f);
		globalRect.width = width;
	}
	else if ((anchorMode & AnchorMode::Right) != AnchorMode::Empty)
	{
		globalRect.x = parentRect.x + x + parentRect.width;
		globalRect.width = width;
	}
	else
	{
		// stretch
		globalRect.x = parentRect.x + left;
		globalRect.width = parentRect.width - right - globalRect.x;
	}

	if ((anchorMode & AnchorMode::Top) != AnchorMode::Empty)
	{
		globalRect.y = parentRect.y + y;
		globalRect.height = height;
	}
	else if ((anchorMode & AnchorMode::Middle) != AnchorMode::Empty)
	{
		globalRect.y = parentRect.y + y + (parentRect.height * 0.5f);
		globalRect.height = height;
	}
	else if ((anchorMode & AnchorMode::Bottom) != AnchorMode::Empty)
	{
		globalRect.y = parentRect.y + y + parentRect.height;
		globalRect.height = height;
	}
	else
	{
		// stretch
		globalRect.y = parentRect.y + top;
		globalRect.height = parentRect.height - bottom - globalRect.y;
	}
}

void Minty::UITransformComponent::serialize(Writer& writer) const
{
	writer.write("anchor", anchorMode);

	if ((anchorMode & (AnchorMode::Left | AnchorMode::Center | AnchorMode::Right)) == AnchorMode::All)
	{
		// stretching horizontally
		writer.write("left", left);
		writer.write("right", right);
	}
	else
	{
		// fixed horizontally
		writer.write("x", x);
		writer.write("width", width);
	}

	if ((anchorMode & (AnchorMode::Top | AnchorMode::Middle | AnchorMode::Bottom)) == AnchorMode::All)
	{
		// stretching vertically
		writer.write("top", top);
		writer.write("bottom", bottom);
	}
	else
	{
		// fixed vertically
		writer.write("y", y);
		writer.write("height", height);
	}

	writer.write("z", z);
}

void Minty::UITransformComponent::deserialize(Reader& reader)
{
	reader.read("anchor", anchorMode);

	if (!reader.read("x", x))
		reader.read("left", left);
	if (!reader.read("y", y))
		reader.read("top", top);
	reader.read("z", z);

	if (!reader.read("width", width))
		reader.read("right", right);
	if (!reader.read("height", height))
		reader.read("bottom", bottom);
}
