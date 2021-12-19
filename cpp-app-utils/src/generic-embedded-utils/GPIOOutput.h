/*
 * GPIOOutput.h
 *
 *  Created on: 22.09.2018
 *      Author: joe
 */

#ifndef SRC_GPIOOUTPUT_H_
#define SRC_GPIOOUTPUT_H_

#include "generic-embedded-utils/BasicGPIO.h"

namespace GenericEmbeddedUtils
{

class GPIOOutput : public BasicGPIO {
public:
	class BlinkSequence
	{
	public:
		BlinkSequence(bool v, const int *t, int c) :
			initialValue(v),toggleTimeInMSAry(t),toggleTimeAryCnt(c) {}

		bool initialValue;

		const int *toggleTimeInMSAry;

		int toggleTimeAryCnt;
	};

private:
	const BlinkSequence *activeBlinkSequence;

	bool value;

	int blinkSequenceToggleTimeAryCntr;

	int blinkTimerId;

public:
	GPIOOutput(int gpioNumber, int exportTimeoutUs, bool initialValue);

	GPIOOutput(int gpioNumber, int exportTimeoutUs, const BlinkSequence *activeBlinkSequence);

	virtual ~GPIOOutput();

	bool Init();

	void SetModeConstantValue(bool value);

	void SetModeBlinking(const BlinkSequence *sequence);

	bool GetCurrentValue();

private:
	static gboolean OnTimeoutElapsed(gpointer user_data);

	void DisableTimer();

	void SetValue(bool value);
};

}

#endif /* SRC_GPIOOUTPUT_H_ */
