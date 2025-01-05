#include "pch.h"
#include "Window.h"

#include "Minty/Script/ScriptEngine.h"
#include "Minty/Script/ScriptClass.h"
#include "Minty/Script/ScriptObject.h"
#include "Minty/Script/ScriptArguments.h"

#ifdef MINTY_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

using namespace Minty;

Minty::Window::Window(WindowBuilder const& builder)
	: m_id(UUID::create())
	, m_data(WindowData{
.title = builder.title,
.width = builder.width,
.height = builder.height
		})
	, m_scriptClass(ScriptEngine::find_class(MINTY_NAME_SCRIPT_NAMESPACE, "Window"))
{}

Owner<Window> Minty::Window::create(WindowBuilder const& builder)
{
#ifdef MINTY_WINDOWS
	return Owner<WindowsWindow>(builder);
#else
	MINTY_ERROR("Unknown platform: cannot create Window.");
	return nullptr;
#endif
}