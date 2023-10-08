#pragma once

#include "settings.hpp"
#include "print.hpp"

#include <sstream>

#define LOG_INFO(format, ...)  shell::Logger::getInstance().write(shell::LogLevel::Info, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) shell::Logger::getInstance().write(shell::LogLevel::Error, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...)                                                                                       \
	shell::Logger::getInstance().write(shell::LogLevel::Warning, format, ##__VA_ARGS__)

#ifdef DEBUG
// todo only print `[DEBUG]` at start of line (i.e. handle flushing)
#define LOG_DEBUG(format, ...) shell::Logger::getInstance().write(shell::LogLevel::Debug, format, ##__VA_ARGS__)
#else
#define LOG_DEBUG(format, ...)
#endif

namespace shell {

/*!
 * @brief -.
*/
enum class LogLevel { Info, Warning, Error, Debug };

#ifdef DEBUG
	constexpr auto g_logLevel = LogLevel::Debug;
#else
	constexpr auto g_logLevel = LogLevel::Error;
#endif

constexpr auto GetLogLevelPrefix(LogLevel logLevel)
{
	switch (logLevel) {
		case LogLevel::Info:
			return "[Info]: ";
		case LogLevel::Warning:
			return "[Warning]: ";
		case LogLevel::Error:
			return "[Error]: ";
		case LogLevel::Debug:
			return "[Debug]: ";
	};
}

/*!
 * @brief Singleton Logging class.
 */
class Logger {

	/*!
	 * @brief -.
	*/
	Logger() = default;

public:

	/*!
	 * @brief Writes message like printf but with loglevel prefix (use like tprintf).
	 * @param logLevel
	 * @param format
	 * @param args
	 */
	template <class... Args>
	void write(LogLevel logLevel, const char *format, Args &&...args) {
		tprintf(GetLogLevelPrefix(logLevel));
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

/*!
 * @brief Buffered logging class
*/
class BLogger
{
	const LogLevel logLevel; /*!< -. */
	std::stringstream buffer; /*!< -. */

public:

	/*!
	 * @brief Buffer will be flushed on destruction of object.
	 * 		  Nothing will be written when the global loglevel is lower than the loglevel you try to write with.
	 * @param logLevel
	*/
	BLogger(LogLevel logLevel_) : logLevel(logLevel_)
	{
		if (logLevel <= g_logLevel)
		{
			buffer << GetLogLevelPrefix(logLevel);
		}
	}

	/*!
	 * @brief .-
	*/
	~BLogger()
	{
		flush();
	}

	/*!
	 * @brief Adds value to buffer.
	*/
	template<class T>
	BLogger& operator << (const T& value)
	{
		if (logLevel <= g_logLevel)
		{
			buffer << value;
		}
		return *this;
	}

	/*!
	 * @brief flushes the buffer.
	*/
	void flush()
	{
		std::cout << buffer.str();
		buffer.clear();
	}

};

} // namespace shell
