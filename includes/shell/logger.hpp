#pragma once

#include "shell/settings.hpp"
#include "shell/print.hpp"
#include "shell/util.hpp"

#include <sstream>

#define LOG_INFO(format, ...)    shell::SLogger::getInstance().write(shell::LogLevel::Info, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...)   shell::SLogger::getInstance().write(shell::LogLevel::Error, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) shell::SLogger::getInstance().write(shell::LogLevel::Warning, format, ##__VA_ARGS__)

#ifdef DEBUG
#define LOG_DEBUG(format, ...) shell::SLogger::getInstance().write(shell::LogLevel::Debug, format, ##__VA_ARGS__)
#else
#define LOG_DEBUG(format, ...)
#endif

namespace shell {

/*!
 * @brief -.
 */
enum class LogLevel { Info, Warning, Error, Debug };

/*!
 * @brief Logger baseclass can only be instantiated by derived class.
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
class SLogger : public Logger {

	/*!
	 * @brief -.
	 */
	SLogger() = default;

public:
	/*!
	 * @brief Writes message like printf but with loglevel prefix (use like tprintf).
	 * @param logLevel
	 * @param format
	 * @param args
	 */
	template <class... Args>
	void write(LogLevel logLevel, const char *format, Args &&...args) {
		tprintf("%", getLogLevelPrefix(logLevel));
		tprintf(format, std::forward<Args>(args)...);
	}

	/*!
	 * @brief Get singleton instance.
	 * @return
	 */
	static SLogger &getInstance();
};

/*!
 * @brief Buffered logging class
 */
class BLogger : public Logger {
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
	BLogger(LogLevel logLevel_);

	/*!
	 * @brief .-
	 */
	~BLogger();

	/*!
	 * @brief Adds value to buffer.
	 */
	template <class T>
	BLogger &operator<<(const T &val) {
		addToBuffer(val);
		return *this;
	}

	/*!
	 * @brief flushes the buffer.
	 */
	void flush();
};

} // namespace shell
