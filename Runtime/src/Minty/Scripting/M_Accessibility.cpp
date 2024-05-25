#include "pch.h"
#include "Minty/Scripting/M_Accessibility.h"

using namespace Minty;
using namespace Minty::Scripting;

Accessibility Minty::Scripting::operator|(Accessibility const left, Accessibility const right)
{
    return static_cast<Accessibility>(static_cast<uint8_t>(left) | static_cast<uint8_t>(right));
}

Accessibility Minty::Scripting::operator&(Accessibility const left, Accessibility const right)
{
    return static_cast<Accessibility>(static_cast<uint8_t>(left) & static_cast<uint8_t>(right));
}