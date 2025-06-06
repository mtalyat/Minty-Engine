#include "pch.h"
#include "M_KeyAction.h"

using namespace Minty;

String Minty::to_string(KeyAction const value)
{
    switch (value) {
    case KeyAction::Up: return "Up";
    case KeyAction::Down: return "Down";
    case KeyAction::Hold: return "Hold";
    default: return "";
    }
}
