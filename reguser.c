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

	fd = open("/dev/read_temp", O_RDWR);
    if (fd < 0) {
        perror("open");
        exit(-2);
    }
	for(; ch < 8; ch++)
	{
		write_reg(ch, 0, 0x30);
		write_reg(ch, 0x8, 0x200|0x100|0x1);
		write_reg(ch, 0x8, 0x200|0x1);
		if(read_reg(ch) != 0)
		{
			printf("Read temp failed");			
			return -1;
		}
		write_reg(ch, 0x8, 0x400|0x200|0x1);
		write_reg(ch, 0x8, 0x200|0x1);
		write_reg(ch, 0, 0x110);
		if(read_reg(ch) != 0)
		{
			printf("Read temp failed");			
			return -1;
		}
		reg_low = userdata.rval&0xfff;
		temp = reg_low*0.0489-40.0+0.625;
		printf("ch: %d, temp: %lf\n", ch, temp);
	}
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
        perror("ioctl wrong");
        return -1;
    }
	return 0;
}
static int read_reg(uint8_t ch)
{
	userdata.ch = ch;
	userdata.rval = 0;
	userdata.offset = 0x8;	
	userdata.wval = 0;
	int ret = ioctl(fd,READREG,&userdata);
    if (ret) {
        perror("ioctl wrong");
        return -1;
    }
	return 0;
}
