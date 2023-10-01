#pragma once

#include "print.hpp"

#ifdef DEBUG
# define LOG_ERROR(format, ...) shell::Logger::getInstance().write(shell::Logger::LogLevel::Error, format, ##__VA_ARGS__)
# define LOG_WARNING(format, ...) shell::Logger::getInstance().write(shell::Logger::LogLevel::Warning, format, ##__VA_ARGS__)
# define LOG_INFO(format, ...) shell::Logger::getInstance().write(shell::Logger::LogLevel::Info, format, ##__VA_ARGS__)
#else
# define LOG_ERROR(format, ...)
# define LOG_Warning(format, ...)
# define LOG_INFO(format, ...)
#endif


namespace shell
{

class Logger
{
	Logger() = default;

public:

	enum class LogLevel { Info, Warning, Error };

	template<class... Args>
	void write(LogLevel logLevel, const char* format, Args&&... args)
	{
		switch(logLevel)
		{
			case LogLevel::Info:
				tprintf("[Info]: ");
				break;
			case LogLevel::Warning:
				tprintf("[Warning]: ");
				break;
			case LogLevel::Error:
				tprintf("[Error]: ");
				break;
		};
		tprintf(format, std::forward<Args>(args)...);
	}

	static Logger& getInstance()
	{
		static Logger logger;
		return logger;
	}
};

} // namespace shell
