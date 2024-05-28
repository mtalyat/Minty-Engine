#include "pch.h"
#include "Minty/Types/M_Logger.h"

#include "Minty/Files/M_PhysicalFile.h"
#include "Minty/Types/M_Time.h"

using namespace Minty;

Minty::Logger::Logger()
	: _path("Log.txt")
	, _firstTime(true)
	, _autoFlush()
{
	_file = new PhysicalFile(_path, File::Flags::Write | File::Flags::Truncate);
}

Minty::Logger::Logger(Path const& path)
	: _path(path)
	, _firstTime(true)
	, _autoFlush()
{
	_file = new PhysicalFile(_path, File::Flags::Write | File::Flags::Truncate);
}

Minty::Logger::Logger(Path const& path, bool const autoFlush)
	: _path(path)
	, _firstTime(true)
	, _autoFlush(autoFlush)
{
	_file = new PhysicalFile(_path, File::Flags::Write | File::Flags::Truncate);
}

Minty::Logger::~Logger()
{
	if (_file)
	{
		flush();
		_file->close();
		delete _file;
	}
}

void Minty::Logger::write(String const& text)
{
	String line = std::format("[{}] {}\n", Time::timestamp(), text);
	_file->write(line.c_str(), line.size());
}

void Minty::Logger::log(String const& text)
{
	write(text);

	if (_autoFlush)
	{
		flush();
	}
}

void Minty::Logger::log_warning(String const& text)
{
	log(std::format("<WARNING> {}", text));
}

void Minty::Logger::log_error(String const& text)
{
	log(std::format("<ERROR> {}", text));
}

void Minty::Logger::log_info(String const& text)
{
	log(std::format("<INFO> {}", text));
}

void Minty::Logger::log_todo(String const& text)
{
	log(std::format("<TODO> {}", text));
}

void Minty::Logger::flush()
{
	_file->flush();
}
