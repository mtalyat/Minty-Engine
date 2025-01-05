#include "pch.h"
#include "AnchorMode.h"

using namespace Minty;

AnchorMode Minty::operator|(AnchorMode const left, AnchorMode const right)
{
    return static_cast<AnchorMode>(static_cast<int>(left) | static_cast<int>(right));
}

AnchorMode Minty::operator&(AnchorMode const left, AnchorMode const right)
{
    return static_cast<AnchorMode>(static_cast<int>(left) & static_cast<int>(right));
}
