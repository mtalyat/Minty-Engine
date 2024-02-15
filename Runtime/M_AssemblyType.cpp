#include "pch.h"
#include "M_AssemblyType.h"

#include "M_Text.h"

using namespace minty;

String minty::to_string(AssemblyType const value)
{
    switch (value)
    {
    case AssemblyType::Engine: return "Engine";
    case AssemblyType::Editor: return "Editor";
    case AssemblyType::Project: return "Project";
    default: return Text::EMPTY;
    }
}
