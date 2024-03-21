#include "pch.h"
#include "M_Accessibility.h"

using namespace minty;
using namespace minty::Scripting;

Accessibility minty::Scripting::operator|(Accessibility const left, Accessibility const right)
{
    return static_cast<Accessibility>(static_cast<uint8_t>(left) | static_cast<uint8_t>(right));
}

Accessibility minty::Scripting::operator&(Accessibility const left, Accessibility const right)
{
    return static_cast<Accessibility>(static_cast<uint8_t>(left) & static_cast<uint8_t>(right));
}