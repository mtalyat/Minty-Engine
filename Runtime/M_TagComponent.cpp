#include "pch.h"
#include "M_TagComponent.h"

#include "M_Writer.h"
#include "M_Reader.h"

using namespace minty;

void minty::TagComponent::serialize(Writer& writer) const
{
	writer.write("tag", tag);
}

void minty::TagComponent::deserialize(Reader const& reader)
{
	reader.try_read_string("tag", tag);
}
