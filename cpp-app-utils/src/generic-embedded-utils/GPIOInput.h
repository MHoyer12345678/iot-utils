/*
 * GPIOInput.h
 *
 *  Created on: 22.09.2018
 *      Author: joe
 */

#ifndef SRC_GPIOINPUT_H_
#define SRC_GPIOINPUT_H_

#include "generic-embedded-utils/BasicGPIO.h"

namespace GenericEmbeddedUtils
{

class GPIOInput : public BasicGPIO {

public:
	class IGpioValueListener
		{
		public:
			/**
			 * Called when the value has changed.
			 * @param gpio the instance of GPIOInput with the changed value
			 * @param value the new value
			 */
			virtual void OnValueChanged(GPIOInput *gpio, bool value)=0;

			/**
			 * returns poll interval in us to sample the value from gpio port. Return 0 to enable reading the value on interrupt.
			 * @return poll interval in us to sample the value from gpio port. Return 0 to enable reading the value on interrupt.
			 */
			virtual int GetPollIntervalUs()=0;
		};

private:
	IGpioValueListener *listener;

	int eventSrcId;

	int valueEventFd;

	bool value;

public:
	GPIOInput(int gpioNumber, int exportTimeoutUs);

	GPIOInput(int gpioNumber, int exportTimeoutUs, IGpioValueListener *listener);

	virtual ~GPIOInput();

	virtual bool Init();

	void EnableNotification();

	void DisableNotification();

	bool GetValue();

private:
	void EnableInterruptEventHandling();

	void EnablePollingEventHandling(int pollIntervalUs);

	static gboolean OnPollGPIOValue(gpointer user_data);

	static gboolean OnIntrGPIOValue(gint fd,GIOCondition condition,gpointer user_data);

};

}

#endif /* SRC_GPIOINPUT_H_ */
