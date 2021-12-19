/*
 * logger.cpp
 *
 *  Created on: 27.11.2016
 *      Author: joe
 */

#include "cpp-app-utils/Logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define DEFAULT_LOGLEVEL 			Logger::INFO

namespace CppAppUtils {


Logger::LogLevel Logger::logLevel=DEFAULT_LOGLEVEL;

Logger::Logger()
{

}

Logger::~Logger()
{

}

void Logger::SetLogLevel(LogLevel logLevel)
{
	Logger::logLevel=logLevel;
}

void Logger::DoLog(Logger::LogLevel level, const char *format, ...)
{
	if (level <= Logger::logLevel)
	{
		FILE *log_stream;
		va_list args;

		if (level == Logger::ERROR)
			log_stream = stderr;
		else
			log_stream = stdout;

		if (level == Logger::DEBUG || level == Logger::VERBOSE_DEBUG)
			fprintf(log_stream,"[DEBUG]: ");
		else if (level == Logger::INFO)
			fprintf(log_stream,"[INFO]: ");
		else if (level == Logger::ERROR)
			fprintf(log_stream,"[ERROR]: ");

		va_start (args, format);
		vfprintf(log_stream, format, args);
		va_end (args);
		fprintf(log_stream,"\n");
		fflush(log_stream);
	}
}

bool Logger::ParseLogLevel(const char *levelStr)
{
	if (strcmp(levelStr,"vdebug")==0 || strcmp(levelStr,"VDEBUG")==0)
		Logger::logLevel=Logger::VERBOSE_DEBUG;
	else if (strcmp(levelStr,"debug")==0 || strcmp(levelStr,"DEBUG")==0)
		Logger::logLevel=Logger::DEBUG;
	else if (strcmp(levelStr,"info")==0 || strcmp(levelStr,"INFO")==0)
		Logger::logLevel=Logger::INFO;
	else if (strcmp(levelStr,"error")==0 || strcmp(levelStr,"ERROR")==0)
		Logger::logLevel=Logger::ERROR;
	else
		return false;

	return true;
}

}
