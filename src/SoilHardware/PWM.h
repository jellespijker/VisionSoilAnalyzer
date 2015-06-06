#pragma once
#include "BBB.h"
#include <dirent.h>

#define OCP_PATH "/sys/devices/ocp.3/"
#define P8_13_FIND "bs_pwm_test_P8_13" 
#define P8_19_FIND "bs_pwm_test_P8_19"
#define P9_14_FIND "bs_pwm_test_P9_14"
#define P9_16_FIND "bs_pwm_test_P9_16"

#define PWM_CAPE "am33xx_pwm"
#define P8_13_CAPE "bspwm_P8_13" //_14
#define P8_19_CAPE "bspwm_P8_13" //_14
#define P9_14_CAPE "bspwm_P9_14" //_16
#define P9_16_CAPE "bspwm_P9_16" //_16

#define P8_13_CAPE_LOAD "bspwm_P8_13_14"
#define P8_19_CAPE_LOAD "bspwm_P8_13_14"
#define P9_14_CAPE_LOAD "bspwm_P9_14_16"
#define P9_16_CAPE_LOAD "bspwm_P9_16_16"

namespace Hardware
{
	class PWM :
		public BBB
	{
	public:
		enum Pin // Four possible PWM pins
		{
			P8_13,
			P8_19,
			P9_14,
			P9_16
		};
		enum Run // Signal generating
		{
			On = 1,
			Off = 0
		};
		enum Polarity // Inverse duty polarity
		{
			Normal = 1,
			Inverted = 0
		};

		Pin pin; // Current pin

		uint8_t GetPixelValue() { return pixelvalue; }
		void SetPixelValue(uint8_t value);

		float GetIntensity() { return intensity; };
		void SetIntensity(float value);

		int GetPeriod() { return period; };
		void SetPeriod(int value);

		int GetDuty() { return duty; };
		void SetDuty(int value);
		void SetIntensity();


		Run GetRun() { return run; };
		void SetRun(Run value);

		Polarity GetPolarity() { return polarity; };
		void SetPolarity(Polarity value);

		PWM(Pin pin);
		~PWM();

	private:
		int period; // current period
		int duty; // current duty
		float intensity; // current intensity
		uint8_t pixelvalue; // current pixelvalue
		Run run; // current run state
		Polarity polarity; // current polaity 

		string basepath; // the basepath ocp.3
		string dutypath; // base + duty path
		string periodpath; // base + period path
		string runpath; // base + run path
		string polaritypath; // base + polarity path

		void calcIntensity();
		string FindPath(string value);

	};
}
