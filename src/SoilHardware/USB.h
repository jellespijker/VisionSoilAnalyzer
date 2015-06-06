#pragma once

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

#include <linux/usbdevice_fs.h>

namespace Hardware
{
	class USB
	{
	public:
		USB();
		~USB();
		void ResetUSB();
	};
}
