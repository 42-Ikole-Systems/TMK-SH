#pragma once

enum class LogLevel { None, Info, Warning, Debug };

namespace shell {

class Logger {
private:
	Logger(LogLevel global_log_level_) : global_log_level(global_log_level_) {
	}

public:
	LogLevel global_log_level;

	template <typename T>
	Logger &operator<<(const T &value) {
		std::err << value;
		return *this;
	}

	static Logger &Log(LogLevel log_level) {
		static Logger logger(log_level);

		constexpr if (log_level < global_log_level) {
			// disable logger?
		}

		return logger;
	}
};

} // namespace shell