/*
 * GPIOOutput.cpp
 *
 *  Created on: 22.09.2018
 *      Author: joe
 */
#include "generic-embedded-utils/GPIOOutput.h"

#include <stdio.h>
#include <glib.h>

#include <cpp-app-utils/Logger.h>

using namespace CppAppUtils;
using namespace GenericEmbeddedUtils;

GPIOOutput::GPIOOutput(int gpioNumber, int exportTimeoutUs, const BlinkSequence *activeBlinkSequence) :
		BasicGPIO(gpioNumber, OUTPUT, exportTimeoutUs),
		value(false),
		blinkSequenceToggleTimeAryCntr(0),
		blinkTimerId(0),
		activeBlinkSequence(activeBlinkSequence)
{

}

GPIOOutput::GPIOOutput(int gpioNumber, int exportTimeoutUs, bool initialValue) :
		BasicGPIO(gpioNumber, OUTPUT, exportTimeoutUs),
		value(initialValue),
		blinkSequenceToggleTimeAryCntr(0),
		blinkTimerId(0),
		activeBlinkSequence(NULL)
{

}

GPIOOutput::~GPIOOutput()
{
	this->DisableTimer();
}

bool GPIOOutput::Init()
{
	if (!BasicGPIO::Init())
		return false;

	if (this->activeBlinkSequence==NULL)
		//initially in constant mode
		this->SetModeConstantValue(this->value);
	else
		this->SetModeBlinking(activeBlinkSequence);

	return true;
}

void GPIOOutput::SetModeConstantValue(bool value)
{
	Logger::LogDebug("GPIOOutput::SetModeConstantValue -> Disabling timer and setting value to: %d", value);
	this->DisableTimer();
	this->blinkSequenceToggleTimeAryCntr=0;
	this->activeBlinkSequence=NULL;

	this->SetValue(value);
}


void GPIOOutput::SetModeBlinking(const BlinkSequence *sequence)
{
	//reset to new sequence starting
	this->DisableTimer();
	this->blinkSequenceToggleTimeAryCntr=0;
	this->activeBlinkSequence=sequence;

	this->SetValue(sequence->initialValue);

	this->blinkTimerId=g_timeout_add(sequence->toggleTimeInMSAry[0],
			GPIOOutput::OnTimeoutElapsed,this);
}

bool GPIOOutput::GetCurrentValue()
{
	return this->value;
}

gboolean GPIOOutput::OnTimeoutElapsed(gpointer user_data)
{
	GPIOOutput *instance=(GPIOOutput *)user_data;
	int newTimeout;

	instance->blinkSequenceToggleTimeAryCntr++;
	if (instance->blinkSequenceToggleTimeAryCntr<instance->activeBlinkSequence->toggleTimeAryCnt)
	{
		instance->SetValue(!instance->value);
	}
	else
	{
		instance->SetValue(instance->activeBlinkSequence->initialValue);
		instance->blinkSequenceToggleTimeAryCntr=0;
	}

	newTimeout=instance->activeBlinkSequence->toggleTimeInMSAry[instance->blinkSequenceToggleTimeAryCntr];

	instance->blinkTimerId=g_timeout_add(newTimeout,GPIOOutput::OnTimeoutElapsed,instance);

	//ensure destroying the previous timeout source attached by returning false
	return FALSE;
}

void GPIOOutput::DisableTimer()
{
	if (this->blinkTimerId!=0)
	{
		g_source_remove(this->blinkTimerId);
		this->blinkTimerId=0;
	}
}

void GPIOOutput::SetValue(bool value)
{
	int gpioStrLen;
	char gpioStr[2], path[2048];

	Logger::LogDebug("GPIOOutput::SetValue -> Setting gpio %d to value %d.",this->GetGPIONumber(), value);
	this->value=value;

	this->CreateValuePath(path,2048);

	gpioStrLen=snprintf(gpioStr, 2, "%d",value);

	if (!this->WriteValue(path,gpioStr,gpioStrLen))
		Logger::LogError("Unable to write value %d to value file: %s",value, path);
}
