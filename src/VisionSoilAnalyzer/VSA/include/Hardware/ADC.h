/*! \class ADC
Interaction with the beaglebone analogue pins
*/

#pragma once

#include "BBB.h"
#include "ADCReadException.h"

#define ADC0_PATH "/sys/bus/iio/devices/iio:device0/in_voltage0_raw" /*!< path to analogue pin 0*/
#define ADC1_PATH "/sys/bus/iio/devices/iio:device0/in_voltage1_raw" /*!< path to analogue pin 1*/
#define ADC2_PATH "/sys/bus/iio/devices/iio:device0/in_voltage2_raw" /*!< path to analogue pin 2*/
#define ADC3_PATH "/sys/bus/iio/devices/iio:device0/in_voltage3_raw" /*!< path to analogue pin 3*/
#define ADC4_PATH "/sys/bus/iio/devices/iio:device0/in_voltage4_raw" /*!< path to analogue pin 4*/
#define ADC5_PATH "/sys/bus/iio/devices/iio:device0/in_voltage5_raw" /*!< path to analogue pin 5*/
#define ADC6_PATH "/sys/bus/iio/devices/iio:device0/in_voltage6_raw" /*!< path to analogue pin 6*/
#define ADC7_PATH "/sys/bus/iio/devices/iio:device0/in_voltage7_raw" /*!< path to analogue pin 7*/

namespace Hardware
{
	class ADC :
		public BBB
	{
	public:
		/*! Enumerator to indicate the analogue pin*/
		enum ADCPin
		{
			ADC0, /*!< AIN0 pin*/
			ADC1, /*!< AIN1 pin*/
			ADC2, /*!< AIN2 pin*/
			ADC3, /*!< AIN3 pin*/
			ADC4, /*!< AIN4 pin*/
			ADC5, /*!< AIN5 pin*/
			ADC6, /*!< AIN6 pin*/
			ADC7 /*!< AIN7 pin*/
		};

		ADCPin Pin; /*!< current pin*/

		ADC(ADCPin pin);
		~ADC();

		int GetCurrentValue();
		float GetIntensity() { return Intensity; }
		int GetMinIntensity() { return MinIntensity; }
		int GetMaxIntensity() { return MaxIntensity; }

		void SetMinIntensity();
		void SetMaxIntensity();

		int WaitForValueChange();
		int WaitForValueChange(CallbackType callback);
		void WaitForValueChangeCancel() { this->threadRunning = false; }

	private:
		string adcpath;		/*!< Path to analogue write file*/
		float Intensity;	/*!< Current intensity expressed as percentage*/
		int MinIntensity;	/*!< Voltage level which represent 0 percentage*/
		int MaxIntensity;	/*!< Voltage level which represent 100 percentage*/

		friend void *threadedPollADC(void *value); /*!< friend polling function*/

	};

	void *threadedPollADC(void *value);
}
