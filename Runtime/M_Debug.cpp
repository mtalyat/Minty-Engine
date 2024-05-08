#include "pch.h"
#include "M_Debug.h"

#include "M_Console.h"
#include "M_Logger.h"

using namespace minty;

static Logger _logger("Log.txt", true);

void minty::Debug::log(String const& text)
{
	Console::log(text);
	_logger.log(text);
}

void minty::Debug::log_warning(String const& text)
{
	Console::warn(text);
	_logger.log_warning(text);
}

void minty::Debug::log_error(String const& text)
{
	Console::error(text);
	_logger.log_error(text);
}

void minty::Debug::log_info(String const& text)
{
	Console::info(text);
	_logger.log_info(text);
}

void minty::Debug::log_todo(String const& text)
{
	Console::todo(text);
	_logger.log_todo(text);
}
