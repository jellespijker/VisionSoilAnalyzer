#include "USB.h"

namespace Hardware
{
	USB::USB()
	{
	}


	USB::~USB()
	{
	}


	void USB::ResetUSB()
	{
		int fd, rc;
		
		fd = open("/dev/bus/usb/001/002", O_WRONLY);
		rc = ioctl(fd, USBDEVFS_RESET, 0);
		if (rc < 0)
		{
			throw - 1;
		}
		close(fd);
	}
}