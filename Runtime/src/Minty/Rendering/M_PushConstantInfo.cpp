#include "pch.h"
#include "Minty/Rendering/M_PushConstantInfo.h"

#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

using namespace Minty;

void Minty::PushConstantInfo::serialize(Writer& writer) const
{
	writer.write("name", name);
	writer.write("stageFlags", vk::to_string(stageFlags));
	writer.write("offset", offset);
	writer.write("size", size);
}

void Minty::PushConstantInfo::deserialize(Reader const& reader)
{
	name = reader.read_string("name");
	stageFlags = vk::from_string_vk_shader_stage_flag_bits(reader.read_string("stageFlags"));
	offset = reader.read_uint("offset");
	size = reader.read_uint("size");
}
