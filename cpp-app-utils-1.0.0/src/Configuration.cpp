/*
 * Configuration.cpp
 *
 *  Created on: 28.11.2016
 *      Author: joe
 */

#include "cpp-app-utils/Configuration.h"
#include <sysexits.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

namespace CppAppUtils {

Configuration::Configuration(const char *defaultConfFile, Logger::LogLevel defaultLogLevel) :
logLevelFoundInCmdline(false),
confFile(defaultConfFile),
defaultConfFile(defaultConfFile)
{
	Logger::SetLogLevel(defaultLogLevel);
}

Configuration::~Configuration()
{
}

bool Configuration::ParseArgsEarly(int argc, char* argv[], int& exitResult)
{
	bool result=true;
	int i;

	Logger::LogDebug("Configuration::ParseArgsEarly -> Parsing command line args (1st stage).");
	for (i=1;i<argc && result;i++)
	{
		if (strcmp(argv[i],"-l")==0 || strcmp(argv[i],"--loglevel")==0)
		{
			this->logLevelFoundInCmdline=true;
			result=this->ParseLoglevelArg(argv,i,argc, exitResult);
		}
		else if (strcmp(argv[i],"-c")==0 || strcmp(argv[i],"--conf")==0)
			result=this->ParseConfFileArg(argv,i,argc, exitResult);
		else if (strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--help")==0)
		{
			this->PrintHelp();
			result=false;
		}
		else if (!this->ParseArgEarlyCustom(argv,i,argc,exitResult))
			return false;
		else
		{
			std::cout << "Unknown command line option: " << argv[i]<<std::endl;
			this->PrintUsage();
			exitResult=EX_USAGE;
			result=false;
		}
	}

	return result;
}

bool Configuration::ParseArgEarlyCustom(char* argv[], int &curArgIdx, int argc, int& exitResult)
{
	//no additional options by default. Override to add some
	return true;
}


bool Configuration::ReadConfigurationFile()
{
	GKeyFile *confFile;
    GError *error=NULL;
    bool result=true;

    gsize numKeys=0, numGroups=0;
    gchar **keys, **groups, *value;

    Logger::LogInfo("Using configuration file: %s",this->confFile);
	Logger::LogDebug("Configuration::ReadConf -> Reading out configuration file.");

	confFile = g_key_file_new();
    if(!g_key_file_load_from_file(confFile,this->confFile,G_KEY_FILE_NONE,&error))
	{
    	g_key_file_free(confFile);
    	if (strcmp(this->confFile,this->defaultConfFile)!=0)
    	{
    		Logger::LogError("Unable to read configuration file: %s",error->message);
    		g_error_free(error);
    		return false;
    	}
    	else
    	{
    		Logger::LogInfo("Default configuration file not found. Using default configuration values.");
    		g_error_free(error);
			return true;
    	}
	}

    groups = g_key_file_get_groups(confFile, &numGroups);
    for(int group = 0;group < numGroups && result;group++)
    {
    	Logger::LogDebug("Configuration::ReadConfigurationFile -> Found group [%s].",groups[group]);
		if (this->IsConfigFileGroupKnown(groups[group]))
		{
			keys = g_key_file_get_keys(confFile, groups[group], &numKeys, NULL);
			for (int key=0;key<numKeys && result;key++)
			{
		    	Logger::LogDebug("Configuration::ReadConfigurationFile -> Found key '%s'.",keys[key]);
				result=this->ParseConfigFileItem(confFile, groups[group], keys[key]);
			}
			if (keys!=NULL)
				g_strfreev(keys);
		}
		else
			Logger::LogError("Unknown section [%s] found in configuration file. Ignoring it.", groups[group]);
    }

	if (groups != NULL)
		g_strfreev(groups);

	g_key_file_free(confFile);

	return result;
}

void Configuration::AddConfigurationModule(IConfigurationParserModule *configModule)
{
	this->configModuleList.push_back(configModule);
}


bool Configuration::IsConfigFileGroupKnown(const char *group)
{
	for (std::list<IConfigurationParserModule *>::iterator i=this->configModuleList.begin();
			i!=this->configModuleList.end(); ++i)
	{
		Configuration::IConfigurationParserModule *configModule=*i;
		if (configModule->IsConfigFileGroupKnown(group))
			return true;
	}

	return false;
}

bool Configuration::ParseConfigFileItem(GKeyFile *confFile, const char *group, const char *key)
{
	for (std::list<IConfigurationParserModule *>::iterator i=this->configModuleList.begin();
			i!=this->configModuleList.end(); ++i)
	{
		Configuration::IConfigurationParserModule *configModule=*i;
		if (!configModule->ParseConfigFileItem(confFile,group,key))
			return false;
	}

	return true;
}

char *Configuration::TrimString(char *str)
{
	char *begin=str, *end;

	//find start of trimmed string
	while (*begin==' ' || *begin=='\t')
		begin++;
	end = begin;

	//goto end of string
	while (*end!='\0')
		end++;
	end--;

	//fill up string from end to end of trimmed string with '\0'
	while (*end==' ' || *end=='\t')
	{
		*end='\0';
		end--;
	}

	return begin;
}

bool Configuration::ParseLoglevelArg(char* argv[], int &curArgIdx, int argc, int& exitResult)
{
	curArgIdx++;

	if (curArgIdx==argc)
	{
		std::cout << "Loglevel missing." << std::endl;
		this->PrintUsage();
		return false;
	}

	if (!Logger::ParseLogLevel(argv[curArgIdx]))
	{
		std::cout << "Invalid log level: " << argv[curArgIdx] << std::endl;
		return false;
	}

	Logger::LogDebug("Configuration::ParseLoglevelArgs -> Setting log level \'%s\'.",argv[curArgIdx]);

	return true;
}

bool Configuration::ParseConfFileArg(char* argv[], int &curArgIdx, int argc, int& exitResult)
{
	curArgIdx++;

	if (curArgIdx==argc)
	{
		std::cout << "Configuration file missing." << std::endl;
		this->PrintUsage();
		return false;
	}

	this->confFile=argv[curArgIdx];

	return true;
}

void Configuration::PrintUsage()
{
	std::cout << "Usage: " << this->GetCommand() << " [-c <conffile> ] [-l <log level>] [-h] "<<this->GetCustomArgsString()<<std::endl<<std::endl;
}

void Configuration::PrintHelp()
{
	std::cout << this->GetDescriptionString()<<std::endl;
	std::cout << this->GetCommand() << " [-c <conffile> ] [-l <log level>] [-h] " << this->GetCustomArgsString()<<std::endl;
	std::cout << "\t-c,--conf <conffile>: use the passed configuration file (default: "<<this->defaultConfFile<<")"<<std::endl;
	std::cout << "\t-l,--loglevel <loglevel>: starts the daemon with the desired log level"<<std::endl;
	std::cout << "\t\t\tERROR: only errors are logged."<<std::endl;
	std::cout << "\t\t\tINFO: only info messages and errors are logged."<<std::endl;
	std::cout << "\t\t\tDEBUG: creates debug messages."<<std::endl;
	std::cout << "\t\t\tVDEBUG: creates lots of debug messages."<<std::endl;
	std::cout << "\t-h,--help: Prints this help."<<std::endl;
	this->PrintCustomHelp();
	std::cout << std::endl;
}

const char *Configuration::GetCustomArgsString()
{
	//override to add options to the args string shown in usage and help screen
	return "";
}

void Configuration::PrintCustomHelp()
{
	//override to print help descriptions of additional options shown in help screen
}

bool Configuration::GetStringValueFromKey(GKeyFile *confFileObj, const char *key,
		const char *group, char **valuePtr)
{
	GError *error=NULL;
	char *value;

	value=g_key_file_get_string(confFileObj, group, key, &error);
	if (value!=NULL)
		*valuePtr=value;

	if (error != NULL)
	{
		Logger::LogError("Unable to get value from key '%s' in group '%s': %s", key,group, error->message);
		g_error_free(error);
		return false;
	}
	else
		return true;
}

bool Configuration::GetInt64ValueFromKey(GKeyFile *confFileObj, const char *key,
		const char *group, int *valuePtr)
{
	GError *error=NULL;
	int value;

	value=g_key_file_get_uint64(confFileObj, group, key, &error);

	if (error != NULL)
	{
		Logger::LogError("Value assigned to key '%s' in group [%s] needs to be a number: %s", key,group, error->message);
		g_error_free(error);
		return false;
	}
	else
	{
		*valuePtr=value;
		return true;
	}
}

bool Configuration::GetBooleanValueFromKey(GKeyFile *confFileObj,
		const char *key, const char *group, bool *valuePtr)
{
	GError *error=NULL;
	bool value;

	value=g_key_file_get_boolean(confFileObj, group, key, &error);

	if (error != NULL)
	{
		Logger::LogError("Value assigned to key '%s' in group [%s] needs to be a boolean: %s", key,group, error->message);
		g_error_free(error);
		return false;
	}
	else
	{
		*valuePtr=value;
		return true;
	}
}

} /* namespace ServerPM */

