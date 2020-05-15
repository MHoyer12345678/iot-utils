/*
 * gpio.h
 *
 *  Created on: 03.08.2017
 *      Author: joe
 */

#ifndef SRC_BASICGPIO_H_
#define SRC_BASICGPIO_H_

#include <stdbool.h>

#include <glib.h>

namespace GenericEmbeddedUtils
{

class BasicGPIO
{
protected:
	enum PortMode
	{
		OUTPUT,
		INPUT
	};

public:
	BasicGPIO(int gpioNumber, PortMode mode, int exportTimeoutUs);

	virtual ~BasicGPIO();

	virtual bool Init();

	int GetGPIONumber();

private:
	int gpioNumber;

	int exportTimeoutUs;

	PortMode mode;

	bool WaitForExport(int timeout_us);

	bool SetPortMode();

	bool Export();

protected:
	void CreateValuePath(char *path, int maxPathLen);

	void CreatePathToFile(char *path, int maxPathLen, const char *fn);

	bool ReadFromPath(char* path, char *buf, int buflen);

	bool WriteValue(const char *path, const char* value, int val_len);

	bool ReadValueAsBool(bool* valuePtr);
};

}

#endif /* SRC_BASICGPIO_H_ */
