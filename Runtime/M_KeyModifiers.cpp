#include "pch.h"
#include "M_KeyModifiers.h"

using namespace minty;

String minty::to_string(KeyModifiers const value)
{
    switch (value) {
    case KeyModifiers::Shift: return "Shift";
    case KeyModifiers::Control: return "Control";
    case KeyModifiers::Alt: return "Alt";
    case KeyModifiers::Super: return "Super";
    case KeyModifiers::CapsLock: return "CapsLock";
    case KeyModifiers::NumLock: return "NumLock";
    default: return Text::EMPTY;
    }
}
