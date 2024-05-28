#include "pch.h"
#include "Minty/Components/M_TagComponent.h"

#include "Minty/Serialization/M_Writer.h"
#include "Minty/Serialization/M_Reader.h"

using namespace Minty;

void Minty::TagComponent::serialize(Writer& writer) const
{
	writer.write("tag", tag);
}

void Minty::TagComponent::deserialize(Reader const& reader)
{
	reader.try_read_string("tag", tag);
}
