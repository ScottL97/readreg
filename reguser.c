#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ioctlreg.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

struct ioctl_data userdata;
int fd;
static int read_reg(uint8_t ch);
static int write_reg(uint8_t ch, uint32_t offset, uint32_t wval);
int main(void)
{
	uint8_t ch = 0;
	uint32_t reg_low = 0;
	double temp = 0;

	fd = open("/dev/read_i350", O_RDWR);
    if (fd < 0) {
        perror("open");
        exit(-2);
    }
	//write_reg(0, 0x9d, 0xff);
	if(read_reg(0) != 0)
	{
		printf("Read i350 failed");			
		return -1;
	}
	printf("value: %x\n", userdata.rval);
	close(fd);
}
static int write_reg(uint8_t ch, uint32_t offset, uint32_t wval)
{
	userdata.ch = ch;
	userdata.rval = 0;
	userdata.offset = offset;
	userdata.wval = wval;
	int ret = ioctl(fd,WRITEREG,&userdata);
    if (ret) {
        perror("ioctl write wrong");
        return -1;
    }
	return 0;
}
static int read_reg(uint8_t ch)
{
	userdata.ch = ch;
	userdata.rval = 0;
	userdata.offset = 0x9d;	
	userdata.wval = 0;
	int ret = ioctl(fd,READREG,&userdata);
    if (ret) {
        perror("ioctl read wrong");
        return -1;
    }
	return 0;
}
