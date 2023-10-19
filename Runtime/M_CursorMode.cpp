#include "pch.h"
#include "M_CursorMode.h"

std::string minty::to_string(CursorMode const mode)
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
        return minty::error::ERROR_TEXT;
    }
}
