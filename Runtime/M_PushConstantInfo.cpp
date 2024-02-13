#include "pch.h"
#include "M_PushConstantInfo.h"

using namespace minty;

void minty::PushConstantInfo::serialize(Writer& writer) const
{
	writer.write("name", name);
	writer.write("stageFlags", vk::to_string(stageFlags));
	writer.write("offset", offset);
	writer.write("size", size);
}

void minty::PushConstantInfo::deserialize(Reader const& reader)
{
	name = reader.read_string("name");
	stageFlags = vk::from_string_vk_shader_stage_flag_bits(reader.read_string("stageFlags"));
	offset = reader.read_uint("offset");
	size = reader.read_uint("size");
}
