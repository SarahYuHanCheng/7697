#include <Arduino.h>

#include "TCS3200.h"

TCS3200::TCS3200()
{
	// Empty constructor.
}

TCS3200::TCS3200(uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t out) :
	_S0_pin(S0), _S1_pin(S1), _S2_pin(S2), _S3_pin(S3), _out_pin(out)
{
	pinMode(_S0_pin, OUTPUT);
	pinMode(_S1_pin, OUTPUT);
	pinMode(_S2_pin, OUTPUT);
	pinMode(_S3_pin, OUTPUT);
	pinMode(_out_pin, INPUT);
}

void TCS3200::setOutFreqScaling(OutFreqScaling scaling)
{
	switch (scaling) {
	case POWER_DOWN:
		digitalWrite(_S0_pin, LOW);
		digitalWrite(_S1_pin, LOW);
		break;
	case PERCENT_2:
		digitalWrite(_S0_pin, LOW);
		digitalWrite(_S1_pin, HIGH);
		break;
	case PERCENT_20:
		digitalWrite(_S0_pin, HIGH);
		digitalWrite(_S1_pin, LOW);
		break;
	case PERCENT_100:
		digitalWrite(_S0_pin, HIGH);
		digitalWrite(_S1_pin, HIGH);
		break;
	}
}

unsigned long TCS3200::getPeriod(PhotodiodeType type)
{
	switch (type) {
	case RED:
		digitalWrite(_S2_pin, LOW);
		digitalWrite(_S3_pin, LOW);
		break;
	case BLUE:
		digitalWrite(_S2_pin, LOW);
		digitalWrite(_S3_pin, HIGH);
		break;
	case CLEAR:
		digitalWrite(_S2_pin, HIGH);
		digitalWrite(_S3_pin, LOW);
		break;
	case GREEN:
		digitalWrite(_S2_pin, HIGH);
		digitalWrite(_S3_pin, HIGH);
		break;
	}

	/* TCS3200 generates 50% duty-cycle pulse. */
	return pulseIn(_out_pin, LOW) << 1;
}

double TCS3200::getFrequency(PhotodiodeType type)
{
	// getPeriod() will return the period in us.
	return 1000000.0f / getPeriod(type);
}
