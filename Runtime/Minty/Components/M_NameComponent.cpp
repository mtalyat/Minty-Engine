#include "pch.h"
#include "Minty/Components/M_NameComponent.h"

#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"

using namespace Minty;

void Minty::NameComponent::serialize(Writer& writer) const
{
	writer.write("name", name);
}

void Minty::NameComponent::deserialize(Reader const& reader)
{
	reader.try_read_string("name", name);
}