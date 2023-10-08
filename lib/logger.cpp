
#include "shell/logger.hpp"

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
	switch (logLevel) {
		case LogLevel::Info:
			return string("[Info]: ");
		case LogLevel::Warning:
			return string("[Warning]: ");
		case LogLevel::Error:
			return string("[Error]: ");
		case LogLevel::Debug:
			return string("[Debug]: ");
	};
}

SLogger &SLogger::getInstance() {
	static SLogger logger;
	return logger;
}

BLogger::BLogger(LogLevel logLevel_) : logLevel(logLevel_) {
	addToBuffer(getLogLevelPrefix(logLevel));
}

BLogger::~BLogger() {
	flush();
}

void BLogger::flush() {
	std::cout << buffer.str();
	buffer.clear();
	addToBuffer(getLogLevelPrefix(logLevel));
}

} // namespace shell
