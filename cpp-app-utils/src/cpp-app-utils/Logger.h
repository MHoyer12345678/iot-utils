/*
 * logger.h
 *
 *  Created on: 27.11.2016
 *      Author: joe
 */

#ifndef LOGGER_H_
#define LOGGER_H_

namespace CppAppUtils {

#define LogError(format, ...) DoLog(Logger::ERROR,format, ##__VA_ARGS__)

#define LogInfo(format, ...) DoLog(Logger::INFO,format,##__VA_ARGS__)

#define LogDebug(format, ...) DoLog(Logger::DEBUG,format,##__VA_ARGS__)

#define LogVerboseDebug(format, ...) DoLog(Logger::VERBOSE_DEBUG,format,##__VA_ARGS__)

class Logger {
public:
	enum LogLevel
	{
		ERROR=0,
		INFO,
		DEBUG,
		VERBOSE_DEBUG
	};

private:
	static Logger::LogLevel logLevel;


private:
	Logger();
	virtual ~Logger();

public:
	static bool ParseLogLevel(const char *levelStr);

	static void SetLogLevel(LogLevel logLevel);

	static void DoLog(LogLevel level, const char *format, ...)
	                __attribute__ ((format (printf, 2, 3)));

};

} /* namespace CppAppUtils */

#endif /* LOGGER_H_ */
