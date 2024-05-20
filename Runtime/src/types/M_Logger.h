#pragma once
#include "core/M_Base.h"

#include <vector>

namespace minty
{
	class File;

	/// <summary>
	/// Logs data to a file.
	/// </summary>
	class Logger
	{
	private:
		// the path to write to
		Path _path;

		File* _file;

		// when true, on first flush, write instead of append
		bool _firstTime;

		// automatically flush after each log
		bool _autoFlush;

	public:
		Logger();

		Logger(Path const& path);

		Logger(Path const& path, bool const autoFlush);

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