#include "pch.h"
#include "M_UITransformComponent.h"

#include "M_Reader.h"
#include "M_Writer.h"

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

AnchorMode minty::operator|(AnchorMode const left, AnchorMode const right)
{
	return static_cast<AnchorMode>(static_cast<int>(left) | static_cast<int>(right));
}

AnchorMode minty::operator&(AnchorMode const left, AnchorMode const right)
{
	return static_cast<AnchorMode>(static_cast<int>(left) & static_cast<int>(right));
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
	return std::format("SpriteComponent(anchorMode = {}, x/left = {}, y/top = {}, width/right = {}, height/bottom = {})", static_cast<Byte>(value.anchorMode), value.x, value.y, value.width, value.height);
}

void minty::UITransformComponent::update_global_rect(RectF const parentRect)
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

void minty::UITransformComponent::serialize(Writer& writer) const
{
	writer.write("anchorMode", to_string(anchorMode));

	// write differently based on the anchor mode

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
}

void minty::UITransformComponent::deserialize(Reader const& reader)
{
	String name;
	if (reader.try_read_string("anchorMode", name))anchorMode = from_string_anchor_mode(name);
	reader.try_read_float("x", x);
	reader.try_read_float("left", left);
	reader.try_read_float("y", y);
	reader.try_read_float("top", top);
	reader.try_read_float("width", width);
	reader.try_read_float("right", right);
	reader.try_read_float("height", height);
	reader.try_read_float("bottom", bottom);
}