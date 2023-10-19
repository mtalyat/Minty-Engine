#include "pch.h"
#include "M_KeyAction.h"

std::string minty::to_string(KeyAction const value)
{
    switch (value) {
    case KeyAction::Up: return "Up";
    case KeyAction::Down: return "Down";
    case KeyAction::Hold: return "Hold";
    default: return minty::error::ERROR_TEXT;
    }
}
