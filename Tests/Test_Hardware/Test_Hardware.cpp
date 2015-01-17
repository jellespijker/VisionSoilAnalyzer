#include <iostream>
#include <sys/time.h>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../../src/VisionSoilAnalyzer/Hardware/Hardware.h"

using namespace std;
using namespace cv;

bool stop = false;
int btnpressed = 0;
int Position;

Hardware::PWM threeLeds{ Hardware::PWM::P9_14 };
Hardware::SoilCape Test;


void DisplayHelp()
{
	cout << "You pressed help" << endl;
}

void TestNormalCam(Mat &procImg)
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	Hardware::Microscope Test(3, Hardware::Microscope::Resolution{ 2592, 1944 });

	clock_gettime(CLOCK_REALTIME, &tstart);
	Test.GetFrame(procImg);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = (unsigned int)(tstop.tv_nsec - tstart.tv_nsec);
	cout << "Capturing of 1 frame : " << elapsedTime << " [ns] " << endl;
}

int TestThreadBtnPressed(int var)
{
	if (btnpressed++ > 1)
	{
		cout << "Button is pressed twice... now exiting!" << endl;
	}
	else
	{
		cout << "Button is pressed once, press again to exit!" << endl;
	}
	return 0;
}

void TestGPIO()
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	clock_gettime(CLOCK_REALTIME, &tstart);
	Hardware::GPIO test(115);
	test.SetDirection(Hardware::GPIO::Input);
	test.SetEdge(Hardware::GPIO::Rising);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = (unsigned int)(tstop.tv_nsec - tstart.tv_nsec);
	cout << "Setup of GPIO took : " << elapsedTime << " [ns] " << endl;
	cout << "Entering loop to check polling and threading" << endl;
	test.WaitForEdge(&TestThreadBtnPressed);
	while (btnpressed <= 1)
	{
		cout << "Doing other things!!!!" << endl;
		sleep(1);
	}
}

int TestThreadRotaryChanged(int var)
{
	if (var != Position)
	{
		Position = var;
		cout << "the rotated value is now :" << var << endl;
	}
	return 0;
}

void TestRotaryEncoder()
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	Hardware::eQEP Test(eQEP2, Hardware::eQEP::eQEP_Mode_Absolute);
	Test.set_period(100000000L);

	Test.WaitForPositionChange(&TestThreadRotaryChanged);
	while (1)
	{
		cout << "doing stuff" << endl;
		usleep(50000);
	}

}

void TestPWM()
{
	Hardware::PWM Test(Hardware::PWM::P9_14);
	int duty = Test.GetDuty();
	int period = Test.GetPeriod();
	int run = Test.GetRun();
	int polarity = Test.GetPolarity();
	float intensity = Test.GetPolarity();

	Test.SetPixelValue(0);
	sleep(5);

	for (uint8_t i = 0; i < 255; i++)
	{
		Test.SetPixelValue(i);
		cout << "current value is : " << (int)i << endl;
		usleep(150000);
	}

	Test.SetRun(Hardware::PWM::Off);

}

void TestEC12P()
{
	Hardware::EC12P Test;
	Test.Rotary.WaitForPositionChange(&TestThreadRotaryChanged);
	Test.Button.WaitForEdge(&TestThreadBtnPressed);
	Test.RainbowLoop(500000);

	while (1)
	{
		cout << "doing normal stuff" << endl;
		usleep(1000000);
	}
}

int ADC2PWM(int var)
{
	cout << "Current value is : " << var << endl;
	threeLeds.SetDuty(4096 - var);
}

void TestADC()
{
	Hardware::ADC Test(Hardware::ADC::ADC0);
	Test.WaitForValueChange(&ADC2PWM);

	while (1)
	{
		cout << "doing normal stuff" << endl;
		usleep(1000000);
	}
}

int CapeBtnPressed(int var)
{
	stop = true;
	return 0;
}

int CapeEncChanged(int var)
{
	Test.MicroscopeLEDs.SetDuty(4096 - var);
	return 0;
}

void TestSoilCape()
{
	Test.RGBEncoder.Button.WaitForEdge(&CapeBtnPressed);
	Test.RGBEncoder.Rotary.WaitForPositionChange(&CapeEncChanged);
	Test.RGBEncoder.RainbowLoop(1000000);

	//cout << "Current duty is: "<< Test.MicroscopeLEDs.GetDuty() << endl;
	//Test.SetLightLevel(0.5);
	//cout << "Current duty is @ 50%: " << Test.MicroscopeLEDs.GetDuty() << endl;
	//Test.SetLightLevel(0.25);
	//cout << "Current duty is @ 25%: " << Test.MicroscopeLEDs.GetDuty() << endl;
	//Test.SetLightLevel(0.75);
	//cout << "Current duty is @ 75%: " << Test.MicroscopeLEDs.GetDuty() << endl;
	//Test.SetLightLevel(1);
	//cout << "Current duty is @ 100%: " << Test.MicroscopeLEDs.GetDuty() << endl;

	//while (!stop)
	//{
	//	cout << "doing normal stuff, current lightlevel = " << Test.MicroscopeLDR.GetCurrentValue() << endl;
	//	usleep(1000000);
	//}

}

int TakeSnapshot(int var)
{
	cv::Mat procImg;
	Hardware::Microscope Test;

	Test.GetFrame(procImg);
	cout << "Snapshot " << ++btnpressed << endl;

	ostringstream ss;
	ss << "/mnt/card/Soil/Debug/image" << btnpressed << ".ppm";
	imwrite(ss.str(), procImg);

}

int ChangeLightLevel(int var)
{
	cout << "current value is :" << Test.MicroscopeLDR.GetCurrentValue();
	Test.MicroscopeLEDs.SetPixelValue(Test.RGBEncoder.Rotary.get_position());
}

void TestMicroscopeCase()
{
	Test.MicroscopeLEDs.SetPixelValue(0);
	Test.RGBEncoder.Button.WaitForEdge(&TakeSnapshot);
	Test.RGBEncoder.Rotary.WaitForPositionChange(&ChangeLightLevel);
	Test.RGBEncoder.RainbowLoop(1000000);

	while (1)
	{
		cout << "doing normal stuff" << endl;
		usleep(2);
	}

}

void TestHDRMicroscope(Mat &img)
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	Hardware::Microscope Test(3, Hardware::Microscope::Resolution{ 2592, 1944 });

	clock_gettime(CLOCK_REALTIME, &tstart);
	Test.GetHDRFrame(img, 3);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = (unsigned int)(tstop.tv_nsec - tstart.tv_nsec);
	cout << "Capturing of 1 HDR frame : " << elapsedTime << " [ns] " << endl;
}

int main(int argc, char *argv[])
{
	Mat img;
	string arg = argv[1];

	if (argc > 0 || arg != "--help")
	{
		for (uint8_t i = 1; i < argc; i++)
		{
			arg = argv[i];
			if (arg == "--NormalCam")
			{
				TestNormalCam(img);
				imwrite("/mnt/card/Soil/Debug/NormalCam.ppm", img);
			}
			else if (arg == "--GPIO")
			{
				TestGPIO();
			}
			else if (arg == "--Encoder")
			{
				TestRotaryEncoder();
			}
			else if (arg == "--PWM")
			{
				TestPWM();
			}
			else if (arg == "--EC12P")
			{
				TestEC12P();
			}
			else if (arg == "--ADC")
			{
				threeLeds.SetDuty(0);
				threeLeds.SetPeriod(4100);
				TestADC();
			}
			else if (arg == "--SoilCape")
			{
				TestSoilCape();
			}
			else if (arg == "--MicroscopeCase")
			{
				TestMicroscopeCase();
			}
			else if (arg == "--HDR")
			{
				TestHDRMicroscope(img);
				imwrite("/mnt/card/Soil/Debug/HDR.ppm", img);
			}
		}
	}
	else
	{
		DisplayHelp();
	}

	return 0;
}