#include "pch.h"
#include "M_MouseButton.h"

using namespace minty;

String minty::to_string(MouseButton const value)
{
    switch (value) {
    case MouseButton::Button1:
        return "Left Mouse Button (1)";
    case MouseButton::Button2:
        return "Right Mouse Button (2)";
    case MouseButton::Button3:
        return "Middle Mouse Button (3)";
    case MouseButton::Button4:
        return "Mouse Button 4";
    case MouseButton::Button5:
        return "Mouse Button 5";
    case MouseButton::Button6:
        return "Mouse Button 6";
    case MouseButton::Button7:
        return "Mouse Button 7";
    case MouseButton::Button8:
        return "Mouse Button 8";
    default:
        return "";
    }
}
