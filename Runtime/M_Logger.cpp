#include "pch.h"
#include "M_Logger.h"

#include "M_PhysicalFile.h"
#include "M_Time.h"

using namespace minty;

minty::Logger::Logger()
	: _path("Log.txt")
	, _firstTime(true)
	, _autoFlush()
{
	_file = new PhysicalFile(_path, File::Flags::Write | File::Flags::Truncate);
}

minty::Logger::Logger(Path const& path)
	: _path(path)
	, _firstTime(true)
	, _autoFlush()
{
	_file = new PhysicalFile(_path, File::Flags::Write | File::Flags::Truncate);
}

minty::Logger::Logger(Path const& path, bool const autoFlush)
	: _path(path)
	, _firstTime(true)
	, _autoFlush(autoFlush)
{
	_file = new PhysicalFile(_path, File::Flags::Write | File::Flags::Truncate);
}

minty::Logger::~Logger()
{
	if (_file)
	{
		flush();
		_file->close();
		delete _file;
	}
}

void minty::Logger::write(String const& text)
{
	String line = std::format("[{}] {}\n", Time::timestamp(), text);
	_file->write(line.c_str(), line.size());
}

void minty::Logger::log(String const& text)
{
	write(text);

	if (_autoFlush)
	{
		flush();
	}
}

void minty::Logger::log_warning(String const& text)
{
	log(std::format("<WARNING> {}", text));
}

void minty::Logger::log_error(String const& text)
{
	log(std::format("<ERROR> {}", text));
}

void minty::Logger::log_info(String const& text)
{
	log(std::format("<INFO> {}", text));
}

void minty::Logger::log_todo(String const& text)
{
	log(std::format("<TODO> {}", text));
}

void minty::Logger::flush()
{
	_file->flush();
}
