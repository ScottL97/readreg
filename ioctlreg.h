#ifndef __IOCTLREG_H
#define __IOCTLREG_H

#include <asm/ioctl.h>
#include <linux/ioctl.h>

struct ioctl_data {
	uint8_t ch;
	uint32_t address;
	uint32_t val;	
};

#define IOC_MAGIC 'c'
//#define _IOR(type,nr,size)	_IOC(_IOC_READ,(type),(nr),sizeof(size))
#define READREG _IOWR(IOC_MAGIC,0,struct ioctl_data)
#define IOC_MAXNR 2

#endif
