#pragma once

#include "settings.hpp"
#include "print.hpp"

#define LOG_INFO(format, ...)  shell::Logger::getInstance().write(shell::Logger::LogLevel::Info, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) shell::Logger::getInstance().write(shell::Logger::LogLevel::Error, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...)                                                                                       \
	shell::Logger::getInstance().write(shell::Logger::LogLevel::Warning, format, ##__VA_ARGS__)

#ifdef DEBUG
// todo only print `[DEBUG]` at start of line (i.e. handle flushing)
#define LOG_DEBUG(format, ...) shell::Logger::getInstance().write(shell::Logger::LogLevel::Debug, format, ##__VA_ARGS__)
#else
#define LOG_DEBUG(format, ...)
#endif

namespace shell {

/*!
 * @brief Manages Logging.
 */
class Logger {
	Logger() = default;

public:
	/*!
	 * @brief -.
	 */
	enum class LogLevel { Info, Warning, Error, Debug };

	/*!
	 * @brief Writes message like printf but with loglevel prefix.
	 * @param logLevel
	 * @param format
	 * @param args
	 */
	template <class... Args>
	void write(LogLevel logLevel, const char *format, Args &&...args) {
		switch (logLevel) {
			case LogLevel::Info:
				tprintf("[Info]: ");
				break;
			case LogLevel::Warning:
				tprintf("[Warning]: ");
				break;
			case LogLevel::Error:
				tprintf("[Error]: ");
				break;
			case LogLevel::Debug:
				tprintf("[Debug]: ");
				break;
		};
		tprintf(format, std::forward<Args>(args)...);
	}

	/*!
	 * @brief Get singleton instance.
	 * @return
	 */
	static Logger &getInstance() {
		static Logger logger;
		return logger;
	}
};

} // namespace shell
