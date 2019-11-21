#ifndef __IOCTLREG_H
#define __IOCTLREG_H

#define IOC_MAGIC 'c'

#define READREG _IOR(IOC_MAGIC,1,int)
#define IOC_MAXNR 2

#endif
