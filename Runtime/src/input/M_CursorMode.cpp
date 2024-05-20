#include "pch.h"
#include "input/M_CursorMode.h"

using namespace minty;

String minty::to_string(CursorMode const mode)
{
    switch (mode)
    {
    case CursorMode::Normal:
        return "Normal";
    case CursorMode::Hidden:
        return "Hidden";
    case CursorMode::Disabled:
        return "Disabled";
    default:
        return "";
    }
}
