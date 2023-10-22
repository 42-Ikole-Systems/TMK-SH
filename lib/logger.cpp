
#include "shell/logger.hpp"
#include "shell/environments/singleton_shell_environment.hpp"

# define COLOR_RESET	 "\033[0m"
# define COLOR_INFO		 "\e[38;5;13m"
# define COLOR_WARNING	 "\e[38;5;208m"
# define COLOR_ERROR	 "\e[38;5;160m"
# define COLOR_DEBUG	 "\e[38;5;21m"

namespace shell {

Logger::Logger()
    : g_logLevel(
#ifdef DEBUG
          LogLevel::Debug
#else
          LogLevel::Error
#endif
      ) {
}

const string Logger::getLogLevelPrefix(LogLevel logLevel) {
	static auto& environment = SingletonShellEnvironment::getInstance();
	static const bool true_color = environment.get("COLORTERM") == "truecolor";
	if (true_color) {
		switch (logLevel) {
			case LogLevel::Info:
				return COLOR_INFO "[Info]: " COLOR_RESET;
			case LogLevel::Warning:
				return COLOR_WARNING "[Warning]: " COLOR_RESET;
			case LogLevel::Error:
				return COLOR_ERROR "[Error]: " COLOR_RESET;
			case LogLevel::Debug:
				return COLOR_DEBUG "[Debug]: " COLOR_RESET;
		};
	}
	else {
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
	return "";
}

SingletonLogger &SingletonLogger::getInstance() {
	static SingletonLogger logger;
	return logger;
}

BufferedLogger::BufferedLogger(LogLevel logLevel_) : logLevel(logLevel_) {
	addToBuffer(getLogLevelPrefix(logLevel));
}

BufferedLogger::~BufferedLogger() {
	flush();
}

void BufferedLogger::flush() {
	std::cout << buffer.str();
	buffer.clear();
	addToBuffer(getLogLevelPrefix(logLevel));
}

} // namespace shell
