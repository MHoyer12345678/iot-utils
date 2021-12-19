/*
 * GPIOInput.cpp
 *
 *  Created on: 22.09.2018
 *      Author: joe
 */

#include "generic-embedded-utils/GPIOInput.h"

#include <glib-unix.h>
#include <unistd.h>
#include <fcntl.h>
#include <cpp-app-utils/Logger.h>

using namespace GenericEmbeddedUtils;
using namespace CppAppUtils;

GPIOInput::GPIOInput(int gpioNumber, int exportTimeoutUs) :
		BasicGPIO(gpioNumber, INPUT, exportTimeoutUs),
		listener(NULL),
		eventSrcId(0),
		value(false),
		valueEventFd(-1)
{

}

GPIOInput::GPIOInput(int gpioNumber, int exportTimeoutUs, IGpioValueListener *listener) :
	BasicGPIO(gpioNumber, INPUT, exportTimeoutUs),
	listener(listener),
	eventSrcId(0),
	value(false),
	valueEventFd(-1)
{

}

GPIOInput::~GPIOInput()
{
	this->DisableNotification();
}

bool GPIOInput::Init()
{

	if (!BasicGPIO::Init())
		return false;

	this->ReadValueAsBool(&this->value);
	Logger::LogDebug("GPIOInput::Init -> Initial value of GPIO %d: %d", this->GetGPIONumber(), this->value);

	if (this->listener!=NULL)
		this->EnableNotification();

	return true;
}

void GPIOInput::EnableNotification()
{
	int pollIntervalUs;

	pollIntervalUs=listener->GetPollIntervalUs();
	if (pollIntervalUs==0)
		this->EnableInterruptEventHandling();
	else
		this->EnablePollingEventHandling(pollIntervalUs);
}

void GPIOInput::DisableNotification()
{
	if (this->eventSrcId!=0)
	{
		g_source_remove(this->eventSrcId);
		this->eventSrcId=0;
	}

	if (this->valueEventFd!=-1)
	{
		close(this->valueEventFd);
		this->valueEventFd=-1;
	}
}

bool GPIOInput::GetValue()
{
	this->ReadValueAsBool(&this->value);
	return this->value;
}

void GPIOInput::EnableInterruptEventHandling()
{
	char path[2048];
	Logger::LogDebug("GPIOInput::EnableInterruptEventHandling -> Enabling interupt based event handling.");

	this->CreateValuePath(path, 2048);

	this->DisableNotification();

	this->valueEventFd = open(path, O_RDONLY | O_NONBLOCK );

	if (this->valueEventFd==-1)
	{
		Logger::LogError("Unable to open file for reading: %s",path);
		return;
	}

	this->eventSrcId=g_unix_fd_add(this->valueEventFd,G_IO_PRI, OnIntrGPIOValue, this);

	this->CreatePathToFile(path, 2048, "edge");
	this->WriteValue(path,"both",4);
}

void GPIOInput::EnablePollingEventHandling(int pollIntervalUs)
{
	Logger::LogDebug("GPIOInput::EnablePollingEventHandling -> Enabling poll based event handling with interval: %d us",pollIntervalUs);
	this->eventSrcId=g_timeout_add(pollIntervalUs/1000,
			GPIOInput::OnPollGPIOValue,this);
}

gboolean GPIOInput::OnPollGPIOValue(gpointer user_data)
{
	GPIOInput *instance=(GPIOInput *)user_data;
	bool oldValue=instance->value;

	if (!instance->ReadValueAsBool(&instance->value))
		return TRUE;

	Logger::LogDebug("GPIOInput::OnPollGPIOValue -> Read out value from GPIO %d: %d",
			instance->GetGPIONumber(), instance->value);

	if (instance->value != oldValue && instance->listener!=NULL)
		instance->listener->OnValueChanged(instance, instance->value);

	return TRUE;
}

gboolean GPIOInput::OnIntrGPIOValue(gint fd,GIOCondition condition,gpointer user_data)
{
	char buf[1024];
	int len;
	GPIOInput *instance=(GPIOInput *)user_data;
	bool oldValue=instance->value;

	lseek(fd, 0, SEEK_SET);
	len=read(fd,buf,1023);
	buf[len]='\0';

	instance->value=(atoi(buf)!=0);
	Logger::LogDebug("GPIOInput::OnIntrGPIOValue -> Read out value from GPIO %d: %d",
			instance->GetGPIONumber(),instance->value);

	if (instance->value != oldValue && instance->listener!=NULL)
		instance->listener->OnValueChanged(instance, instance->value);

	return TRUE;
}
