/**
 * The library for the color sensor TCS3200.
 */
#ifndef _TCS3200_H_
#define _TCS3200_H_

/* Parameters for output frequency scaling. */
enum OutFreqScaling {
	POWER_DOWN,
	PERCENT_2,
	PERCENT_20,
	PERCENT_100
};

/* Parameters for photodiode type. */
enum PhotodiodeType {
	RED,
	BLUE,
	CLEAR,	// No filter
	GREEN
};

class TCS3200
{
public:
	/* Constructor */
	TCS3200();
	TCS3200(uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t out);

	void setOutFreqScaling(OutFreqScaling scaling);
	/**
	 * Get the period of the output pulse.
	 * This method measures only one period.
	 */
	unsigned long getPeriod(PhotodiodeType type);

	/**
	 * Get the frequency of the output pulse.
	 * This method measures only one period.
	 */
	double getFrequency(PhotodiodeType type);

private:
	uint8_t _S0_pin;
	uint8_t _S1_pin;
	uint8_t _S2_pin;
	uint8_t _S3_pin;
	uint8_t _out_pin;
};

#endif // _TCS3200_H_
