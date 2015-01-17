#include "Microscope.h"

//Custom exceptions
#include "MicroscopeNotFoundException.h"
#include "CouldNotGrabImageException.h"

using namespace cv;

namespace Hardware
{
	Microscope::Microscope()
	{
		FrameDelayTrigger = 3;
		Dimensions = Resolution{ 2592, 1944 };

		try { openCam(); }
		catch (Exception::MicroscopeNotFoundException& e)
		{
			// Tries to soft reset the USB port. Haven't got this working yet
			USB usbdev;
			usbdev.ResetUSB();
			captureDevice.open(0);
			if (!captureDevice.isOpened())
			{
				throw Exception::MicroscopeNotFoundException("Soft reset of microscope didn't work. Try turning the soil analyzer on and off again!");
			}
		}
	}

	/*! Constructor of the class which initializes the USB microscope 
	\param frameDelayTrigger the delay between the first initialization of the microscope and the retrivial of the image expressed in seconds. Default value is 3 seconds
	\param dimension A resolution Struct indicating which resolution the webcam should use. Default is 2592 x 1944
	*/
	Microscope::Microscope(uint8_t frameDelayTrigger, Resolution dimensions)
	{
		FrameDelayTrigger = frameDelayTrigger;
		Dimensions = dimensions;

		try {	openCam();	}
		catch (Exception::MicroscopeNotFoundException& e)
		{
			// Tries to soft reset the USB port. Haven't got this working yet
			USB usbdev;
			usbdev.ResetUSB();
			captureDevice.open(0);
			if (!captureDevice.isOpened())
			{
				throw Exception::MicroscopeNotFoundException("Soft reset of microscope didn't work. Try turning the soil analyzer on and off again!");
			}
		}
	}

	/*!< De-constructor*/
	Microscope::~Microscope()
	{
		captureDevice.~VideoCapture();
	}

	/*! Get the frame after the set initialization period
	\param dst a cv::Mat construct which stores the retrieved image
	*/
	void Microscope::GetFrame(cv::Mat &dst)
	{
		if (!captureDevice.grab())	{ throw Exception::CouldNotGrabImageException(); }
		sleep(FrameDelayTrigger); // Needed otherwise scrambled picture
		if (!captureDevice.grab())	{ throw Exception::CouldNotGrabImageException(); }
		captureDevice.retrieve(dst);
	}

	/*! Get an HDR capture of the cam using a user defined number of frames differently lit frames. Due to hardware limitations each frames take roughly 3 seconds to grab. This function is based upon the tutorial from openCV http://docs.opencv.org/trunk/doc/tutorials/photo/hdr_imaging/hdr_imaging.html 
	\param dst a cv::Mat construct with the retrieved HDR result
	\param noframes is the number of frames that create the HDR image - default = 5
	*/
	void Microscope::GetHDRFrame(cv::Mat &dst, uint32_t noframes)
	{
		//create the brightness steps
		int8_t brightnessStep = static_cast<int8_t>((MAX_BRIGHTNESS - MIN_BRIGHTNESS)/ noframes);
		int8_t currentBrightness = captureDevice.get(CV_CAP_PROP_BRIGHTNESS);
		int8_t currentContrast = captureDevice.get(CV_CAP_PROP_CONTRAST);
		captureDevice.set(CV_CAP_PROP_CONTRAST, MAX_CONTRAST);

		Mat currentImg;

		// take the shots at different brightness levels
		for (uint32_t i = 1; i <= noframes; i++)
		{
			captureDevice.set(CV_CAP_PROP_BRIGHTNESS, (MIN_BRIGHTNESS + (i * brightnessStep)));
			GetFrame(currentImg);
			HDRframes.push_back(currentImg);
		}

		// Set the brightness and back to the previous used level
		captureDevice.set(CV_CAP_PROP_BRIGHTNESS, currentBrightness);
		captureDevice.set(CV_CAP_PROP_CONTRAST, currentContrast);

		// Perform the exposure fusion
		Mat fusion;
		Ptr<MergeMertens> merge_mertens = createMergeMertens();
		merge_mertens->process(HDRframes, fusion);
		fusion *= 255;
		fusion.convertTo(dst, CV_8UC1);
	}

	/*!< Checks if the capture device is open and returns the status as a bool
	/return Status of the capture device expressed as a bool
	*/
	bool Microscope::IsOpened()
	{
		return captureDevice.isOpened();
	}

	/*!< Safely release the capture device*/
	void Microscope::Release()
	{
		captureDevice.release();
	}

	/*!< Opens the webcam*/
	void Microscope::openCam()
	{
		captureDevice.open(0);

		if (!captureDevice.isOpened()) { throw Exception::MicroscopeNotFoundException(); }
		captureDevice.set(CV_CAP_PROP_FRAME_WIDTH, Dimensions.Width);
		captureDevice.set(CV_CAP_PROP_FRAME_HEIGHT, Dimensions.Height);
	}
}