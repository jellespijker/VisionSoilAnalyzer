#pragma once

#include "EC12P.h"
#include "GPIO.h"
#include "PWM.h"
#include "ADC.h"

namespace Hardware
{
	class SoilCape
	{
	public:
		EC12P RGBEncoder;
		PWM MicroscopeLEDs{ PWM::P9_14 };
		ADC MicroscopeLDR{ ADC::ADC0 };

		SoilCape();
		~SoilCape();

	};
}
