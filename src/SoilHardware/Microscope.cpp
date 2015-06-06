#include "Microscope.h"

//Custom exceptions
#include "MicroscopeNotFoundException.h"
#include "CouldNotGrabImageException.h"

using namespace cv;
using namespace boost::filesystem;
using namespace std;

namespace Hardware
{

	/*! Constructor of the class which initializes the USB microscope 
	\param frameDelayTrigger the delay between the first initialization of the microscope and the retrivial of the image expressed in seconds. Default value is 3 seconds
	\param dimension A resolution Struct indicating which resolution the webcam should use. Default is 2592 x 1944
	*/
    Microscope::Microscope(uint8_t frameDelayTrigger, Resolution dimensions,  bool firstdefault)
	{
		FrameDelayTrigger = frameDelayTrigger;
		Dimensions = dimensions;

        StartupSeq(firstdefault);
	}

    void Microscope::StartupSeq(bool firstdefault)
    {
        std::vector<std::string> camNames = AvailableCams();
        int videodev = find(camNames.begin(), camNames.end(), "USB Microscope") - camNames.begin();
        if (videodev == camNames.size() && !firstdefault) {throw Exception::MicroscopeNotFoundException("Microscope Not Found Exception!");}
        else if (videodev == camNames.size() && firstdefault) { videodev = 0; }

        struct utsname unameData;
        uname(&unameData);
        arch = static_cast<std::string>(unameData.machine);

        try { openCam(videodev); }
        catch (Exception::MicroscopeNotFoundException& e)
        {
            // Tries to soft reset the USB port. Haven't got this working yet
            USB usbdev;
            usbdev.ResetUSB();
            captureDevice.open(videodev);
            if (!captureDevice.isOpened())
            {
                throw Exception::MicroscopeNotFoundException("USB Soft Reset Exception!");
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
        //Work around for crappy cam retrival of the BBB
        if (arch.find("armv7l") != string::npos)
        {
            if (!captureDevice.grab())	{ throw Exception::CouldNotGrabImageException(); }
            sleep(FrameDelayTrigger); // Needed otherwise scrambled picture
            if (!captureDevice.grab())	{ throw Exception::CouldNotGrabImageException(); }
            captureDevice.retrieve(dst);
        }
        else
        {
            if (!captureDevice.read(dst)) { throw Exception::CouldNotGrabImageException(); }
        }
	}

	/*! Get an HDR capture of the cam using a user defined number of frames differently lit frames. Due to hardware limitations each frames take roughly 3 seconds to grab. This function is based upon the tutorial from openCV http://docs.opencv.org/trunk/doc/tutorials/photo/hdr_imaging/hdr_imaging.html 
	\param dst a cv::Mat construct with the retrieved HDR result
	\param noframes is the number of frames that create the HDR image - default = 5
	*/
	void Microscope::GetHDRFrame(cv::Mat &dst, uint32_t noframes)
	{
        prog_sig(0);
		//create the brightness steps
		int8_t brightnessStep = static_cast<int8_t>((MAX_BRIGHTNESS - MIN_BRIGHTNESS)/ noframes);
		int8_t currentBrightness = captureDevice.get(CV_CAP_PROP_BRIGHTNESS);
		int8_t currentContrast = captureDevice.get(CV_CAP_PROP_CONTRAST);
		captureDevice.set(CV_CAP_PROP_CONTRAST, MAX_CONTRAST);

        int progStep = 70 / noframes;
		Mat currentImg;
		// take the shots at different brightness levels
		for (uint32_t i = 1; i <= noframes; i++)
		{
			captureDevice.set(CV_CAP_PROP_BRIGHTNESS, (MIN_BRIGHTNESS + (i * brightnessStep)));
			GetFrame(currentImg);
			HDRframes.push_back(currentImg);
            prog_sig(i * progStep);
		}

		// Set the brightness and back to the previous used level
		captureDevice.set(CV_CAP_PROP_BRIGHTNESS, currentBrightness);
		captureDevice.set(CV_CAP_PROP_CONTRAST, currentContrast);

		// Perform the exposure fusion
		Mat fusion;
		Ptr<MergeMertens> merge_mertens = createMergeMertens();
		merge_mertens->process(HDRframes, fusion);
        prog_sig(80);
		fusion *= 255;
        prog_sig(85);
		fusion.convertTo(dst, CV_8UC1);
        prog_sig(100);
        fin_sig();
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
    void Microscope::openCam(int dev)
	{
        captureDevice.open(dev);
		if (!captureDevice.isOpened()) { throw Exception::MicroscopeNotFoundException(); }
        captureDevice.set(CV_CAP_PROP_FRAME_WIDTH, Dimensions.Width);
        captureDevice.set(CV_CAP_PROP_FRAME_HEIGHT, Dimensions.Height);
	}

    std::vector<std::string> Microscope::AvailableCams()
    {
        std::vector<std::string> cams;
        const string path_ss = "/sys/class/video4linux";

        if (!exist(path_ss)) return cams;

        for (directory_iterator itr(path_ss); itr!=directory_iterator(); ++itr)
        {
            string videoln = itr->path().string();
            videoln.append("/name");
            if (exist(videoln))
            {
                std::ifstream camName;
                camName.open(videoln);
                std::string name;
                std::getline(camName, name);
                cams.push_back(name);
                camName.close();
            }
        }

        return cams;
    }

    bool Microscope::exist(const string &name)
    {
        struct stat buffer;
        return (stat (name.c_str(), &buffer) == 0);
    }

    boost::signals2::connection Microscope::connect_Finished(const Finished_t::slot_type &subscriber)
    {
        return fin_sig.connect(subscriber);
    }

    boost::signals2::connection Microscope::connect_Progress(const Progress_t::slot_type &subscriber)
    {
        return prog_sig.connect(subscriber);
    }


}
