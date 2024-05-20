#include "pch.h"
#include "components/M_TagComponent.h"

#include "serialization/M_Writer.h"
#include "serialization/M_Reader.h"

using namespace minty;

void minty::TagComponent::serialize(Writer& writer) const
{
	writer.write("tag", tag);
}

void minty::TagComponent::deserialize(Reader const& reader)
{
	reader.try_read_string("tag", tag);
}
