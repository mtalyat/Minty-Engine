#include "pch.h"
#include "Minty/Tools/M_Debug.h"

#include "Minty/Tools/M_Console.h"
#include "Minty/Types/M_Logger.h"

using namespace Minty;

static Logger _logger("Log.txt", true);

void Minty::Debug::log(String const& text)
{
	Console::log(text);
	_logger.log(text);
}

void Minty::Debug::log_warning(String const& text)
{
	Console::warn(text);
	_logger.log_warning(text);
}

void Minty::Debug::log_error(String const& text)
{
	Console::error(text);
	_logger.log_error(text);
}

void Minty::Debug::log_info(String const& text)
{
	Console::info(text);
	_logger.log_info(text);
}

void Minty::Debug::log_todo(String const& text)
{
	Console::todo(text);
	_logger.log_todo(text);
}
