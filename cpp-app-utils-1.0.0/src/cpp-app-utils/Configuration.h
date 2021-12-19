/*
 * Configuration.h
 *
 *  Created on: 28.11.2016
 *      Author: joe
 */

#ifndef SRC_COMMON_CONFIGURATION_H_
#define SRC_COMMON_CONFIGURATION_H_

#include <string>
#include <glib.h>

#include <list>

#include "cpp-app-utils/Logger.h"

namespace CppAppUtils {

class Configuration {
public:
	class IConfigurationParserModule
	{
	public:
		virtual bool ParseConfigFileItem(GKeyFile *confFile, const char *group, const char *key)=0;

		virtual bool IsConfigFileGroupKnown(const char *group)=0;
	};

private:
	const char *confFile;

	const char *defaultConfFile;

	bool logLevelFoundInCmdline;

	std::list<IConfigurationParserModule *> configModuleList;

private:
	bool ParseLoglevelArg(char* argv[], int &curArgIdx, int argc, int& exitResult);

	bool ParseConfFileArg(char* argv[], int &curArgIdx, int argc, int& exitResult);

protected:
	char *TrimString(char *str);

	Configuration(const char *defaultConfFile, Logger::LogLevel defaultLogLevel);

	virtual ~Configuration();

	virtual bool ParseArgEarlyCustom(char* argv[], int &curArgIdx, int argc, int& exitResult);

	virtual const char *GetVersion()=0;

	virtual void PrintUsage();

	virtual void PrintHelp();

	virtual const char *GetDescriptionString()=0;

	virtual const char *GetCommand()=0;

	virtual const char *GetCustomArgsString();

	virtual void PrintCustomHelp();

	virtual bool IsConfigFileGroupKnown(const char *group);

	virtual bool ParseConfigFileItem(GKeyFile *confFile, const char *group, const char *key);

public:
	bool ParseArgsEarly(int argc, char* argv[], int& exitResult);

	bool ReadConfigurationFile();

	void AddConfigurationModule(IConfigurationParserModule *configModule);

	static bool GetStringValueFromKey(GKeyFile *confFileObj, const char *key,
			const char *group, char **valuePtr);

	static bool GetInt64ValueFromKey(GKeyFile *confFileObj, const char *key,
			const char *group, int *valuePtr);

	static bool GetBooleanValueFromKey(GKeyFile *confFileObj, const char *key,
			const char *group, bool *valuePtr);
};

} /* namespace ServerPM */

#endif /* CONFIGURATION_H_ */
