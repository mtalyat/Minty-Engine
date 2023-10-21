#include "pch.h"
#include "M_UIComponent.h"

using namespace minty;

void minty::UIComponent::serialize(Writer& writer) const
{
	writer.write("anchor", static_cast<int>(anchorMode));
	writer.write("x", x, 0.0f);
	writer.write("y", y, 0.0f);
	writer.write("width", width, 0.0f);
	writer.write("height", height, 0.0f);
	writer.write("layer", layer, 0);
	writer.write("sprite", spriteId, 0);
}

void minty::UIComponent::deserialize(Reader const& reader)
{
	anchorMode = static_cast<AnchorMode>(reader.read_int("anchor"));
	x = reader.read_float("x");
	if (x == 0.0f) x = reader.read_float("left");
	y = reader.read_float("y");
	if (y == 0.0f) y = reader.read_float("top");
	width = reader.read_float("width");
	if (width == 0.0f) width = reader.read_float("right");
	height = reader.read_float("height");
	if (height == 0.0f) height = reader.read_float("bottom");
	layer = reader.read_int("layer");
	spriteId = reader.read_id("sprite");
}

std::string minty::to_string(UIComponent const& value)
{
	return std::format("UIComponent(anchor = {}, x/left = {}, y/top = {}, width/right = {}, height/bottom = {}, layer = {}, sprite = {})", static_cast<byte>(value.anchorMode), value.x, value.y, value.width, value.height, value.layer, value.spriteId);
}
