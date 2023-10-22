#pragma once

#include "shell/settings.hpp"
#include "shell/print.hpp"
#include "shell/utility/types.hpp"

#include <sstream>

#define LOG_INFO(format, ...)    shell::SingletonLogger::getInstance().write(shell::LogLevel::Info, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...)   shell::SingletonLogger::getInstance().write(shell::LogLevel::Error, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) shell::SingletonLogger::getInstance().write(shell::LogLevel::Warning, format, ##__VA_ARGS__)

#ifdef DEBUG
#define LOG_DEBUG(format, ...) shell::SingletonLogger::getInstance().write(shell::LogLevel::Debug, format, ##__VA_ARGS__)
#else
#define LOG_DEBUG(format, ...)
#endif

namespace shell {

/*!
 * @brief -.
 */
enum class LogLevel { Info, Warning, Error, Debug };

/*!
 * @brief Logger interface.
 */
class Logger {
protected:
	const LogLevel g_logLevel; /*!< Loglevel set at compiletime. */

	/*!
	 * @brief -.
	 */
	Logger();

	/*!
	 * @brief Gets loglevel prefix as string.
	 */
	static const string getLogLevelPrefix(LogLevel logLevel);
};

/*!
 * @brief Singleton Logging class.
 * @note This implementation is not thread safe.
 */
class SingletonLogger : public Logger {

	/*!
	 * @brief
	 */
	SingletonLogger() = default;

public:
	/*!
	 * @brief Writes message like printf but with loglevel prefix (use like tprintf).
	 * @param logLevel
	 * @param format
	 * @param args
	 */
	template <class... Args>
	void write(LogLevel logLevel, const char *format, Args &&...args) {
		if (logLevel > g_logLevel) {
			return;
		}
		tprintf("%", getLogLevelPrefix(logLevel));
		tprintf(format, std::forward<Args>(args)...);
	}

	/*!
	 * @brief Get singleton instance.
	 * @return
	 */
	static SingletonLogger &getInstance();
};

/*!
 * @brief Buffered logging class
 */
class BufferedLogger : public Logger {
	const LogLevel logLevel;  /*!< -. */
	std::stringstream buffer; /*!< -. */

	template <class T>
	inline void addToBuffer(const T &val) {
		if (logLevel <= g_logLevel) {
			buffer << val;
		}
	}

public:
	/*!
	 * @brief Buffer will be flushed on destruction of object.
	 * 		  Nothing will be written when the global loglevel is lower than the loglevel you try to write with.
	 * @param logLevel
	 */
	BufferedLogger(LogLevel logLevel_);

	/*!
	 * @brief .-
	 */
	~BufferedLogger();

	/*!
	 * @brief Write base function.
	 * @param format
	*/
	void write(const char *format)
	{
		addToBuffer(format);
	}

	/*!
	 * @brief Write base fuction.
	 * @param format
	*/
	void write(const string& format)
	{
		addToBuffer(format);
	}

	/*!
	 * @brief Writes message like printf to buffer.
	 * @param format
	 * @param args
	 */
	template <class T, class... Args>
	void write(const char *format, const T& value, Args &&... Fargs) {
		if (logLevel > g_logLevel) {
			return;
		}
		for (; *format != '\0'; format++) {
		if (*format == '%') {
			buffer << value;
			write(format + 1, Fargs...); // recursive call
			return;
		}
		buffer << *format;
	}

	}

	/*!
	 * @brief Adds value to buffer.
	 */
	template <class T>
	BufferedLogger &operator<<(const T &val) {
		addToBuffer(val);
		return *this;
	}

	/*!
	 * @brief flushes the buffer.
	 */
	void flush();
};

} // namespace shell
