/*
 * gpio.c
 *
 *  Created on: 03.08.2017
 *      Author: joe
 */

#include <generic-embedded-utils/BasicGPIO.h>
#include "cpp-app-utils/Logger.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

#define GPIO_BASE_PATH "/sys/class/gpio"
#define GPIO_EXPORT_FILE "/export"

#define GPIO_WAIT_POLL_TIME_US 	1000

using namespace GenericEmbeddedUtils;
using namespace CppAppUtils;


BasicGPIO::BasicGPIO(int gpio, PortMode mode, int exportTimeoutUs) :
		gpioNumber(gpio),
		mode(mode),
		exportTimeoutUs(exportTimeoutUs)
{

}

BasicGPIO::~BasicGPIO()
{

}

bool BasicGPIO::Init()
{
	bool result;
	result=this->Export();
	if (result)
		result=this->SetPortMode();

	return result;
}

int BasicGPIO::GetGPIONumber()
{
	return this->gpioNumber;
}

bool BasicGPIO::Export()
{
	bool result;

	int gpioStrLen;
	char gpioStr[16];

	//check if already there
	if (this->WaitForExport(GPIO_WAIT_POLL_TIME_US/10))
		return true;

	gpioStrLen=snprintf(gpioStr, 16, "%d",this->gpioNumber);
	result=this->WriteValue(GPIO_BASE_PATH GPIO_EXPORT_FILE,gpioStr,gpioStrLen);

	if (result)
		result = this->WaitForExport(this->exportTimeoutUs);

	if (result)
		Logger::LogDebug("BasicGPIO::Export -> Exported gpio %d.",this->gpioNumber);
	else
		Logger::LogError("Failed to export gpio %d. Code: %d",this->gpioNumber,result);

	return result;
}

bool BasicGPIO::SetPortMode()
{
	bool result;

	char pathStr[2048];
	Logger::LogDebug("BasicGPIO::SetPortModeOutput -> Setting gpio %d to %s mode.",this->gpioNumber, this->mode==OUTPUT ? "output" : "input");
	this->CreatePathToFile(pathStr, 2048, "direction");

	if (this->mode==OUTPUT)
		result=this->WriteValue(pathStr,"out",3);
	else
		result=this->WriteValue(pathStr,"in",2);

	return result;
}

bool BasicGPIO::WriteValue(const char *path, const char* value, int val_len)
{
	bool result=true;
	int fh;

	fh=open(path,O_WRONLY);
	if (fh==-1)
	{
		Logger::LogError("Unable to open path: %s",path);
		return false;
	}

	if (write(fh, value, val_len)!= val_len)
	{
		Logger::LogError("Unable to write value to file.");
		result=false;
	}

	close(fh);

	return result;
}

void BasicGPIO::CreatePathToFile(char *path, int maxPathLen, const char *fn)
{
	snprintf(path, maxPathLen, "%s/gpio%d/%s",GPIO_BASE_PATH, this->gpioNumber, fn);
}

void BasicGPIO::CreateValuePath(char *path, int maxPathLen)
{
	this->CreatePathToFile(path, maxPathLen, "value");
}

bool BasicGPIO::ReadFromPath(char* path, char *buf, int buflen)
{
	bool result=true;
	int fh;

	fh=open(path,O_RDONLY);
	if (fh==-1)
	{
		Logger::LogError("Unable to open path: %s",path);
		return false;
	}

	if (read(fh, buf, buflen)<1)
	{
		Logger::LogError("Unable to read value from file.");
		result=false;
	}

	close(fh);

	return result;
}

bool BasicGPIO::ReadValueAsBool(bool* valuePtr)
{
	int value;
	char buf[16];
	char path[2048];

	this->CreateValuePath(path, 2048);

	if (!this->ReadFromPath(path, buf, 16))
	{
		Logger::LogError("Unable to read value from GPIO: %d", this->gpioNumber);
		return false;
	}

	value=atoi(buf);
	*valuePtr=(value!=0);

	return true;
}

bool BasicGPIO::WaitForExport(int time_out_us)
{
	struct stat statResult;
	char pathStr[2048];
	this->CreatePathToFile(pathStr, 2048, "direction");

	while (time_out_us > 0)
	{
		if (stat(pathStr, &statResult)==0)
			return true;

		usleep(GPIO_WAIT_POLL_TIME_US);
		time_out_us -= GPIO_WAIT_POLL_TIME_US;
	}

	return false;
}
