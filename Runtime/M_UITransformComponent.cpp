#include "pch.h"
#include "M_UITransformComponent.h"

using namespace minty;

String minty::to_string(AnchorMode const anchor)
{
	int mode = static_cast<int>(anchor);
	String result;

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

AnchorMode minty::from_string_anchor_mode(String const& string)
{
	int mode = 0;

	if (string.find("Top") != String::npos)
	{
		mode |= static_cast<int>(AnchorMode::Top);
	}
	if (string.find("Middle") != String::npos)
	{
		mode |= static_cast<int>(AnchorMode::Middle);
	}
	if (string.find("Bottom") != String::npos)
	{
		mode |= static_cast<int>(AnchorMode::Bottom);
	}

	if (string.find("Left") != String::npos)
	{
		mode |= static_cast<int>(AnchorMode::Left);
	}
	if (string.find("Center") != String::npos)
	{
		mode |= static_cast<int>(AnchorMode::Center);
	}
	if (string.find("Right") != String::npos)
	{
		mode |= static_cast<int>(AnchorMode::Right);
	}

	return static_cast<AnchorMode>(mode);
}

String minty::to_string(UITransformComponent const& value)
{
	return std::format("SpriteComponent(anchor = {}, x/left = {}, y/top = {}, width/right = {}, height/bottom = {})", static_cast<Byte>(value.anchorMode), value.x, value.y, value.width, value.height);
}

void minty::UITransformComponent::serialize(Writer& writer) const
{
	writer.write("anchor", to_string(anchorMode));
	writer.write("x", x);
	writer.write("y", y);
	writer.write("width", width);
	writer.write("height", height);
}

void minty::UITransformComponent::deserialize(Reader const& reader)
{
	String name;
	if (reader.try_read_string("anchor", name))anchorMode = from_string_anchor_mode(name);
	reader.try_read_float("x", x);
	reader.try_read_float("left", left);
	reader.try_read_float("y", y);
	reader.try_read_float("top", top);
	reader.try_read_float("width", width);
	reader.try_read_float("right", right);
	reader.try_read_float("height", height);
	reader.try_read_float("bottom", bottom);
}