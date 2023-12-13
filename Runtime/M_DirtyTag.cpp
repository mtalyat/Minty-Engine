#include "pch.h"
#include "M_DirtyTag.h"

std::string minty::to_string(Dirty const& value)
{
    return std::format("Dirty()");
}

void minty::Dirty::serialize(Writer& writer) const
{}

void minty::Dirty::deserialize(Reader const& reader)
{}
