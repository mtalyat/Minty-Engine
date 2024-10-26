#include "pch.h"
#include "Logger.h"

#include "Minty/File/PhysicalFile.h"
#include "Minty/Core/Time.h"

using namespace Minty;

Minty::Logger::Logger()
	: m_path("Log.txt")
	, m_firstTime(true)
	, m_autoFlush(false)
{
	m_file = new PhysicalFile(m_path, File::Flags::Write | File::Flags::Truncate);
}

Minty::Logger::Logger(Path const& path)
	: m_path(path)
	, m_firstTime(true)
	, m_autoFlush(false)
{
	m_file = new PhysicalFile(m_path, File::Flags::Write | File::Flags::Truncate);
}

Minty::Logger::Logger(Path const& path, Bool const autoFlush)
	: m_path(path)
	, m_firstTime(true)
	, m_autoFlush(autoFlush)
{
	m_file = new PhysicalFile(m_path, File::Flags::Write | File::Flags::Truncate);
}

Minty::Logger::~Logger()
{
	if (m_file)
	{
		flush();
		m_file->close();
		delete m_file;
	}
}

void Minty::Logger::write(String const& text)
{
	String line = std::format("[{}] {}\n", Time::timestamp(), text);
	m_file->write(line.c_str(), line.size());
}

void Minty::Logger::log(String const& text)
{
	write(text);

	if (m_autoFlush)
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
	m_file->flush();
}
