#include "pch.h"
#include "M_Writer.h"

#include "M_ISerializable.h"

using namespace minty;

minty::Writer::Writer(SerializedNode& node)
	: _node(node)
{}

void minty::Writer::write(std::string const& name)
{
	write(name, "");
}

void minty::Writer::write(std::string const& name, SerializedNode const& node)
{
	_node.children.emplace(name, node);
}

void minty::Writer::write(std::string const& name, ISerializable const* const value)
{
	// add child object for this object to write
	_node.children.emplace(name, SerializedNode());

	// create a Writer to use
	Writer Writer(_node.children.at(name));

	// serialize the object into that node
	value->serialize(Writer);
}

void minty::Writer::write(std::string const& name, std::string const& value)
{
	_node.children.emplace(name, SerializedNode{ .data = value });
}

void minty::Writer::write(std::string const& name, int const value)
{
	write(name, std::to_string(value));
}

void minty::Writer::write(std::string const& name, float const value)
{
	write(name, std::to_string(value));
}

void minty::Writer::write(std::string const& name, byte const value)
{
	write(name, std::to_string(value));
}

void minty::Writer::write(std::string const& name, Vector2 const& value)
{
	SerializedNode node;

	Writer writer(node);

	writer.write("x", value.x);
	writer.write("y", value.y);

	write(name, node);
}

void minty::Writer::write(std::string const& name, Vector3 const& value)
{
	SerializedNode node;

	Writer writer(node);

	writer.write("x", value.x);
	writer.write("y", value.y);
	writer.write("z", value.z);

	write(name, node);
}

void minty::Writer::write(std::string const& name, Vector4 const& value)
{
	SerializedNode node;

	Writer writer(node);

	writer.write("x", value.x);
	writer.write("y", value.y);
	writer.write("z", value.z);
	writer.write("w", value.w);

	write(name, node);
}

void minty::Writer::write(std::string const& name, Vector2Int const& value)
{
	SerializedNode node;

	Writer writer(node);

	writer.write("x", value.x);
	writer.write("y", value.y);

	write(name, node);
}

void minty::Writer::write(std::string const& name, Vector3Int const& value)
{
	SerializedNode node;

	Writer writer(node);

	writer.write("x", value.x);
	writer.write("y", value.y);
	writer.write("z", value.z);

	write(name, node);
}

void minty::Writer::write(std::string const& name, Vector4Int const& value)
{
	SerializedNode node;

	Writer writer(node);

	writer.write("x", value.x);
	writer.write("y", value.y);
	writer.write("z", value.z);
	writer.write("w", value.w);

	write(name, node);
}

void minty::Writer::write(std::string const& name, Quaternion const& value)
{
	SerializedNode node;

	Writer writer(node);

	Vector3 angles = glm::eulerAngles(value);

	writer.write("x", angles.x); // pitch
	writer.write("y", angles.y); // yaw
	writer.write("z", angles.z); // roll

	write(name, node);
}

void minty::Writer::write(std::string const& name, std::string const& value, std::string const& defaultValue)
{
	if (value.compare(defaultValue))
	{
		// not default value
		_node.children.emplace(name, SerializedNode{ .data = value });
	}
}

void minty::Writer::write(std::string const& name, int const value, int const defaultValue)
{
	if (value != defaultValue)
	{
		write(name, std::to_string(value));
	}
}

void minty::Writer::write(std::string const& name, float const value, float const defaultValue)
{
	if (value != defaultValue)
	{
		write(name, std::to_string(value));
	}
}

void minty::Writer::write(std::string const& name, byte const value, byte const defaultValue)
{
	if (value != defaultValue)
	{
		write(name, std::to_string(value));
	}
}

void minty::Writer::write(std::string const& name, Vector2 const& value, Vector2 const& defaultValue)
{
	if (value != defaultValue)
	{
		SerializedNode node;

		Writer writer(node);

		writer.write("x", value.x, defaultValue.x);
		writer.write("y", value.y, defaultValue.y);

		write(name, node);
	}
}

void minty::Writer::write(std::string const& name, Vector3 const& value, Vector3 const& defaultValue)
{
	if (value != defaultValue)
	{
		SerializedNode node;

		Writer writer(node);

		writer.write("x", value.x, defaultValue.x);
		writer.write("y", value.y, defaultValue.y);
		writer.write("z", value.z, defaultValue.z);

		write(name, node);
	}
}

void minty::Writer::write(std::string const& name, Vector4 const& value, Vector4 const& defaultValue)
{
	if (value != defaultValue)
	{
		SerializedNode node;

		Writer writer(node);

		writer.write("x", value.x, defaultValue.x);
		writer.write("y", value.y, defaultValue.y);
		writer.write("z", value.z, defaultValue.z);
		writer.write("w", value.w, defaultValue.w);

		write(name, node);
	}
}

void minty::Writer::write(std::string const& name, Vector2Int const& value, Vector2Int const& defaultValue)
{
	if (value != defaultValue)
	{
		SerializedNode node;

		Writer writer(node);

		writer.write("x", value.x, defaultValue.x);
		writer.write("y", value.y, defaultValue.y);

		write(name, node);
	}
}

void minty::Writer::write(std::string const& name, Vector3Int const& value, Vector3Int const& defaultValue)
{
	if (value != defaultValue)
	{
		SerializedNode node;

		Writer writer(node);

		writer.write("x", value.x, defaultValue.x);
		writer.write("y", value.y, defaultValue.y);
		writer.write("z", value.z, defaultValue.z);

		write(name, node);
	}
}

void minty::Writer::write(std::string const& name, Vector4Int const& value, Vector4Int const& defaultValue)
{
	if (value != defaultValue)
	{
		SerializedNode node;

		Writer writer(node);

		writer.write("x", value.x, defaultValue.x);
		writer.write("y", value.y, defaultValue.y);
		writer.write("z", value.z, defaultValue.z);
		writer.write("w", value.w, defaultValue.w);

		write(name, node);
	}
}

void minty::Writer::write(std::string const& name, Quaternion const& value, Quaternion const& defaultValue)
{
	if (value != defaultValue)
	{
		SerializedNode node;

		Writer writer(node);

		Vector3 angles = glm::eulerAngles(value);
		Vector3 defaultAngles = glm::eulerAngles(value);

		writer.write("x", angles.x, defaultAngles.x); // pitch
		writer.write("y", angles.y, defaultAngles.y); // yaw
		writer.write("z", angles.z, defaultAngles.z); // roll

		write(name, node);
	}
}
