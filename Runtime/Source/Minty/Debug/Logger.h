#pragma once
#include "Minty/Core/Path.h"
#include "Minty/Core/String.h"

#include <vector>

namespace Minty
{
	class File;

	/// <summary>
	/// Logs data to a file.
	/// </summary>
	class Logger
	{
	private:
		// the path to write to
		Path m_path;

		// the file to write to at the path
		File* mp_file;

		// when true, on first flush, write instead of append
		Bool m_firstTime;

		// automatically flush after each log
		Bool m_autoFlush;

	public:
		Logger(Path const& path);

		Logger(Path const& path, Bool const autoFlush);

		~Logger();

#pragma region Logging

	private:
		void write(String const& text);

	public:
		void log(String const& text);

		template<typename... Args>
		void log_format(String const& format, Args&&... args)
		{
			log(std::format(format, std::forward<Args>(args)...));
		}

		void log_warning(String const& text);

		template<typename... Args>
		void log_warning_format(String const& format, Args&&... args)
		{
			log_warning(std::format(format, std::forward<Args>(args)...));
		}

		void log_error(String const& text);

		template<typename... Args>
		void log_error_format(String const& format, Args&&... args)
		{
			log_error(std::format(format, std::forward<Args>(args)...));
		}

		void log_info(String const& text);

		template<typename... Args>
		void log_info_format(String const& format, Args&&... args)
		{
			log_info(std::format(format, std::forward<Args>(args)...));
		}

		void log_todo(String const& text);

		template<typename... Args>
		void log_todo_format(String const& format, Args&&... args)
		{
			log_todo(std::format(format, std::forward<Args>(args)...));
		}

		void flush();

#pragma endregion
	};
}