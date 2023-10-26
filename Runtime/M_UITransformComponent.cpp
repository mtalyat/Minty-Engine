#include "pch.h"
#include "M_UITransformComponent.h"

using namespace minty;

std::string minty::to_string(AnchorMode const anchor)
{
	int mode = static_cast<int>(anchor);
	std::string result;

	if (mode & static_cast<int>(AnchorMode::Top))
	{
		result += "Top";
	}
	if (mode & static_cast<int>(AnchorMode::Middle))
	{
		result += "Middle";
	}
	if (mode & static_cast<int>(AnchorMode::Bottom))
	{
		result += "Bottom";
	}

	if (mode & static_cast<int>(AnchorMode::Left))
	{
		result += "Left";
	}
	if (mode & static_cast<int>(AnchorMode::Center))
	{
		result += "Center";
	}
	if (mode & static_cast<int>(AnchorMode::Right))
	{
		result += "Right";
	}

	if (!result.size())
	{
		result = "All";
	}

	return result;
}

AnchorMode minty::from_string_anchor_mode(std::string const& string)
{
	int mode = 0;

	if (string.find("Top") != std::string::npos)
	{
		mode |= static_cast<int>(AnchorMode::Top);
	}
	if (string.find("Middle") != std::string::npos)
	{
		mode |= static_cast<int>(AnchorMode::Middle);
	}
	if (string.find("Bottom") != std::string::npos)
	{
		mode |= static_cast<int>(AnchorMode::Bottom);
	}

	if (string.find("Left") != std::string::npos)
	{
		mode |= static_cast<int>(AnchorMode::Left);
	}
	if (string.find("Center") != std::string::npos)
	{
		mode |= static_cast<int>(AnchorMode::Center);
	}
	if (string.find("Right") != std::string::npos)
	{
		mode |= static_cast<int>(AnchorMode::Right);
	}

	return static_cast<AnchorMode>(mode);
}

std::string minty::to_string(UITransformComponent const& value)
{
	return std::format("SpriteComponent(anchor = {}, x/left = {}, y/top = {}, width/right = {}, height/bottom = {})", static_cast<byte>(value.anchorMode), value.x, value.y, value.width, value.height);
}

void minty::UITransformComponent::serialize(Writer& writer) const
{
	writer.write("anchor", to_string(anchorMode));
	writer.write("x", x, 0.0f);
	writer.write("y", y, 0.0f);
	writer.write("width", width, 0.0f);
	writer.write("height", height, 0.0f);
}

void minty::UITransformComponent::deserialize(Reader const& reader)
{
	anchorMode = from_string_anchor_mode(reader.read_string("anchor"));
	x = reader.read_float("x");
	if (x == 0.0f) x = reader.read_float("left");
	y = reader.read_float("y");
	if (y == 0.0f) y = reader.read_float("top");
	width = reader.read_float("width");
	if (width == 0.0f) width = reader.read_float("right");
	height = reader.read_float("height");
	if (height == 0.0f) height = reader.read_float("bottom");
}