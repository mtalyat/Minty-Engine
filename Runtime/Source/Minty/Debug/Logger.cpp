#include "pch.h"
#include "Logger.h"

#include "Minty/File/PhysicalFile.h"
#include "Minty/Core/Time.h"

using namespace Minty;

Minty::Logger::Logger(Path const& path)
	: m_path(path)
	, mp_file(nullptr)
	, m_firstTime(true)
	, m_autoFlush(false)
{
	if (!path.empty())
	{
		mp_file = new PhysicalFile(m_path, File::Flags::Write | File::Flags::Truncate);
	}
}

Minty::Logger::Logger(Path const& path, Bool const autoFlush)
	: m_path(path)
	, mp_file(nullptr)
	, m_firstTime(true)
	, m_autoFlush(autoFlush)
{
	if (!path.empty())
	{
		mp_file = new PhysicalFile(m_path, File::Flags::Write | File::Flags::Truncate);
	}
}

Minty::Logger::~Logger()
{
	if (m_autoFlush)
	{
		flush();
	}
	mp_file->close();
	delete mp_file;
}

void Minty::Logger::write(String const& text)
{
	String line = std::format("[{}] {}\n", Time::timestamp(), text);
	mp_file->write(line.c_str(), line.size());
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
	mp_file->flush();
}
