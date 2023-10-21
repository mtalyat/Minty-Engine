#include "pch.h"
#include "M_DirtyTag.h"

std::string minty::to_string(DirtyTag const& value)
{
    return std::format("DirtyTag()");
}

void minty::DirtyTag::serialize(Writer& writer) const
{}

void minty::DirtyTag::deserialize(Reader const& reader)
{}
